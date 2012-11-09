#include "stdafx.h"

typedef DWORD (WINAPI *extSetProcessWorkingSetSizex)(HANDLE, int,int);
typedef HANDLE (WINAPI *extOpenThreadx)(DWORD, BOOL, DWORD);
typedef DWORD (WINAPI *extResumeProcessx)(HANDLE);
typedef DWORD (WINAPI *extSuspendProcessx)(HANDLE);


extSetProcessWorkingSetSizex extSetProcessWorkingSetSize;
extOpenThreadx extOpenThread;
extResumeProcessx extResumeProcess;
extSuspendProcessx extSuspendProcess;

void __fastcall GetDebugPriv()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
}

HANDLE __fastcall ProcByExe(wchar_t *exe, DWORD &pid)
{
    HANDLE prc=0;
    HANDLE snp=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if( snp != INVALID_HANDLE_VALUE )
    {
        PROCESSENTRY32 pe={sizeof(PROCESSENTRY32), 0};
        if( Process32First(snp, &pe) )
        {
            do
            {
                WCHAR *t = pe.szExeFile + wcslen(pe.szExeFile);
                while( (t > pe.szExeFile) && (*t != '\\') )
                {
                    t--;
                }

                if( *t == '\\' )
                {
                    t++;
                }

                if( ! mystricmp(t, exe) )
                {
                    prc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid = pe.th32ProcessID);
                    break;
                }
            } while (Process32Next(snp, &pe));
        }
        CloseHandle(snp);
    }
    return prc;
}
void __fastcall SuspendResumeIt(DWORD pid, bool suspend)
{
    HANDLE snp = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if(snp != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 pe = {sizeof(THREADENTRY32), 0};
        if(Thread32First(snp, &pe))
        {
            do
            {
                if(pe.th32OwnerProcessID == pid)
                {
                    HANDLE trd = extOpenThread(THREAD_SUSPEND_RESUME, 0, pe.th32ThreadID);
                    if(trd)
                    {
                        if(suspend)
                        {
                            SuspendThread(trd);
                        }
                        else
                        {
                            ResumeThread(trd);
                        }
                        CloseHandle(trd);
                    }
                }
            } while ( Thread32Next(snp, &pe) );
        }
        CloseHandle(snp);
    }
}

void __fastcall CPULimitMain()
{
    if( OpenMutex(MUTEX_ALL_ACCESS, 0, L"CPULimit_Activated_Mutex") )
    {
        MessageBox(0, L"CPULimit.exe already started!", L"CPULimit", MB_ICONWARNING);
        ExitProcess(4294967293);
    }
    HANDLE amtx = CreateMutex(0, true, L"CPULimit_Activated_Mutex");

    extSetProcessWorkingSetSize = (extSetProcessWorkingSetSizex)GetProcAddress(LoadLibrary(L"kernel32.dll"), "SetProcessWorkingSetSize");
    extOpenThread = (extOpenThreadx)GetProcAddress(LoadLibrary(L"kernel32.dll"), "OpenThread");
    extResumeProcess = (extResumeProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtResumeProcess");
    extSuspendProcess = (extSuspendProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtSuspendProcess");


    WCHAR fpath[1100];
    rsize_t fpathsize = 1100;

    GetModuleFileName(0, fpath, 1023); 
    fpath[1023] = 0;
    WCHAR *tmp = fpath + wcslen(fpath);
    while(*tmp!='\\')
    {
        tmp--;
    }
    tmp++;

    wcscpy_s(tmp, fpathsize, L"cpulimit.ini");

    wchar_t exename[261] = {0};
    GetPrivateProfileString(L"Settings", L"ExeName", L"", exename, 260, fpath);
    exename[260] = 0;
    DWORD timeon = GetPrivateProfileInt(L"Settings", L"TimeOn", 1000, fpath);
    DWORD timeoff = GetPrivateProfileInt(L"Settings", L"TimeOff", 1000, fpath);
    DWORD hipr = GetPrivateProfileInt(L"Settings", L"HighPriority", 0, fpath);
    DWORD CanUseNtDll = GetPrivateProfileInt(L"Settings", L"CanUseNtDll", 1, fpath);
    DWORD ExePriority = GetPrivateProfileInt(L"Settings", L"ExePriority", 1, fpath);

    if( exename[0] && (timeon) && (timeoff >= 0) )
    {
        if(extOpenThread)
        {
            GetDebugPriv();
            if(hipr)
            {
                SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
            }
            if(extSetProcessWorkingSetSize)
            {
                extSetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
            }
            HANDLE prc = 0;
            DWORD pid;
            DWORD prc_priority = NORMAL_PRIORITY_CLASS;
            while(! OpenMutex( MUTEX_ALL_ACCESS, 0, L"CPULimit_Deactivate_Mutex") )
            {
                if(prc && (WaitForSingleObject(prc, 0) != WAIT_TIMEOUT) )
                {
                    CloseHandle(prc);
                    prc = 0;
                }
                if(!prc)
                {
                    prc = ProcByExe(exename, pid);

                    if(prc)
                    {
                        switch (ExePriority)
                        {
                        case 0:
                            prc_priority = IDLE_PRIORITY_CLASS;
                            break;
                        case 2:
                            prc_priority = HIGH_PRIORITY_CLASS;
                            break;
                        case 3:
                            prc_priority = REALTIME_PRIORITY_CLASS;
                            break;
                        default:
                            prc_priority = NORMAL_PRIORITY_CLASS;
                            break;
                        }
                        wprintf(L"\n> %s is running.", exename);
                        SetPriorityClass(prc, prc_priority);
                    }
                }
                if(prc)
                {
                    if(timeoff != 0)
                    {
                        if(extSuspendProcess && CanUseNtDll)
                        {
                            extSuspendProcess(prc);
                        }
                        else
                        {
                            SuspendResumeIt(pid, 1);
                        }
                        wprintf(L"!");
                        Sleep(timeoff);
                        wprintf(L".");
                        if(extResumeProcess && CanUseNtDll)
                        {
                            extResumeProcess(prc);
                        }
                        else
                        {
                            SuspendResumeIt(pid, 0);
                        }
                        Sleep(timeon);
                    }
                    else
                    {
                        wprintf(L".");
                        Sleep(2000);
                    }
                }
                else
                {
                    wprintf(L"Wait for openning %s during %u ms...\n", exename, timeoff+timeon);
                    Sleep(timeoff+timeon);
                }
            }
        }
        else
        {
            MessageBox(0, L"Your system is not supported. Exiting.", L"CPU limit", MB_ICONWARNING);
        }
    }
    else
    {
        MessageBox(0, L"CPUlimit.ini is not correct! Please check configuration file. Exiting.", L"CPU limit", MB_ICONWARNING);
    }
    CloseHandle(amtx);
}

void __fastcall HaltMich()
{
    HANDLE mtx;
    if((mtx = OpenMutex(MUTEX_ALL_ACCESS, 0, L"CPULimit_Activated_Mutex")) != NULL)
    {
        CloseHandle(mtx);
        CreateMutex(0, 0, L"CPULimit_Deactivate_Mutex");
        for(;;)
        {
            mtx = OpenMutex(MUTEX_ALL_ACCESS, 0, L"CPULimit_Activated_Mutex");
            if(mtx)
            {
                CloseHandle(mtx);
                Sleep(10);
            }
            else
            {
                break;
            }
        }
    }
}
