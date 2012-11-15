#include "stdafx.h"

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

HANDLE __fastcall ProcByExe(WCHAR *exe, DWORD &pid)
{

    HANDLE prc = 0;
    HANDLE snp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if( snp != INVALID_HANDLE_VALUE )
    {
        PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32), 0};
        if( Process32First(snp, &pe) )
        {
            do
            {
                WCHAR *t = pe.szExeFile + wcslen(pe.szExeFile);
                while( (t > pe.szExeFile) && (*t != '\\') && (*t != '/') )
                {
                    t--;
                }

                if( *t == '\\' && (*t != '/') )
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
        snp = NULL;
    }
    return prc;
}

HANDLE __fastcall ProcById(DWORD pid)
{
    HANDLE prc = 0;
    HANDLE snp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if( snp != INVALID_HANDLE_VALUE )
    {
        PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32), 0};
        if( Process32First(snp, &pe) )
        {
            do
            {
                if( pid == pe.th32ProcessID )
                {
                    prc = OpenProcess(PROCESS_ALL_ACCESS, 0, pe.th32ProcessID);
                    break;
                }
            } while (Process32Next(snp, &pe));
        }
        CloseHandle(snp);
        snp = NULL;
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
                        trd = NULL;
                    }
                }
            } while ( Thread32Next(snp, &pe) );
        }
        CloseHandle(snp);
        snp = NULL;
    }
}

void __fastcall cpulimitMain(int argc, WCHAR *argv[])
{
    if( OpenMutex(MUTEX_ALL_ACCESS, 0, L"CPULimit_Activated_Mutex") )
    {
        fwprintf(stderr, L"cpulimit already started!");
        ExitProcess(4294967293);
    }
    HANDLE amtx = CreateMutex(0, true, L"CPULimit_Activated_Mutex");
    MyExceptionHandler::AddHandle(&amtx);

    extSetProcessWorkingSetSize = (extSetProcessWorkingSetSizex)GetProcAddress(LoadLibrary(L"kernel32.dll"), "SetProcessWorkingSetSize");
    extOpenThread = (extOpenThreadx)GetProcAddress(LoadLibrary(L"kernel32.dll"), "OpenThread");
    extResumeProcess = (extResumeProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtResumeProcess");
    extSuspendProcess = (extSuspendProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtSuspendProcess");


    Config *settings = new Config(argc, argv);
    MyExceptionHandler::SetSettings(settings);

    if(extOpenThread)
    {
        GetDebugPriv();
        if(settings->GetHighPriority())
        {
            SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        }
        if(extSetProcessWorkingSetSize)
        {
            extSetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
        }

        HANDLE prc = 0;
        MyExceptionHandler::AddHandle(&prc);
        DWORD curProcId = GetCurrentProcessId();

        while(! OpenMutex( MUTEX_ALL_ACCESS, 0, L"CPULimit_Deactivate_Mutex") )
        {
            if(prc && (WaitForSingleObject(prc, 0) != WAIT_TIMEOUT) )
            {
                CloseHandle(prc);
                prc = NULL;
            }
            if(!prc)
            {
                prc = process_finder(settings);
            }

            // Control process
            if(prc)
            {
                if(settings->GetProcessId() == curProcId)
                {
                    printf("Target process %d is cpulimit itself! Aborting because it makes no sense\n", curProcId);
                    ExitProcess(EXIT_FAILURE);
                }
                process_limiter(settings, prc);
            }
            else
            {
                // Wait process
                if (settings->GetLazy())
                {
                    break;
                }
                Sleep(2000);
            }
        }
    }
    else
    {
        fwprintf(stderr,L"Your system is not supported. Exiting.");
    }

    CloseHandle(amtx);
    amtx = NULL;
}


void process_limiter(Config *settings, HANDLE prc)
{
    if(settings->GetTimeOff() > 0)
    {
        if(extSuspendProcess && settings->GetNtDll())
        {
            extSuspendProcess(prc);
        }
        else
        {
            SuspendResumeIt(settings->GetProcessId(), 1);
        }

        Sleep(settings->GetTimeOff());

        if(extResumeProcess && settings->GetNtDll())
        {
            extResumeProcess(prc);
        }
        else
        {
            SuspendResumeIt(settings->GetProcessId(), 0);
        }
        Sleep(settings->GetTimeOn());
    }
    else
    {
        // Always running;
        Sleep(2000);
    }
}

HANDLE process_finder(Config *settings)
{
    HANDLE prc = 0;
    DWORD prc_priority = NORMAL_PRIORITY_CLASS;
    DWORD pid = 0;

    if (settings->GetProcessId())
    {
        prc = ProcById(settings->GetProcessId());
    }
    else
    {
        prc = ProcByExe(settings->GetExeName(), pid);
        settings->SetProcessId(pid);
    }

    if(prc)
    {
        fprintf(stdout, "Process %d found.\n", settings->GetProcessId());
        switch (settings->GetCodeExePriority())
        {
        case 0:
            prc_priority = IDLE_PRIORITY_CLASS;
            break;
        default:
            prc_priority = NORMAL_PRIORITY_CLASS;
            break;
        }
        SetPriorityClass(prc, prc_priority);
    }
    else
    {
        fprintf(stderr, "No process found.\n");
    }

    return prc;
}
