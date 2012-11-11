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

void __fastcall CPULimitMain(int argc, WCHAR *argv[])
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


    ClSettings settings = GetSettings();

    //argument variables
    int limit_ok = 0;
    int exe_ok = 0;
    int pid_ok = 0;

    WCHAR *exe = NULL;
    int pid = 0;
    int perclimit = 0;

    //parse arguments
    int next_option;
    int option_index = 0;
    //A string listing valid short options letters
    const WCHAR* short_options = L"+p:e:l:Izh";
    //An array describing valid long options
    const struct option long_options[] = {
        { L"pid", required_argument, NULL, L'p' },
        { L"exe", required_argument, NULL, L'e' },
        { L"limit", required_argument, NULL, L'l' },
        { L"lazy", no_argument, NULL, L'z' },
        { L"idle", no_argument, NULL, L'I' },
        { L"help", no_argument, NULL, L'h' },
        { 0, 0, 0, 0 }
    };
    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, &option_index);
        switch(next_option) 
        {
            case 'p':
                pid = _wtoi(optarg);
                pid_ok = 1;
                break;
            case 'e':
                exe = optarg;
                exe_ok = 1;
                break;
            case 'l':
                perclimit = _wtoi(optarg);
                limit_ok = 1;
                break;
            case 'z':
                settings.lazy = 1;
                break;
            case 'I':
                settings.codeExePriority = 0;
                break;
            case 'h':
                Cmd::PrintUsage(stdout, EXIT_FAILURE);
                break;
            case '?':
                Cmd::PrintUsage(stderr, EXIT_FAILURE);
                break;
            case -1:
                break;
            default:
                abort();
        }
    } while(next_option != -1);

    if (limit_ok)
    {
        if (perclimit < 1 || perclimit > 100)
        {
            fprintf(stderr,"Error: limit must be in the range 1-100\n");
            Cmd::PrintUsage(stderr, EXIT_FAILURE);
        }
        else
        {
            settings.nbTimeOn = perclimit * 10;
            settings.nbTimeOff = 1000 - settings.nbTimeOn;
        }
    }

    if (exe_ok && pid_ok)
    {
        fprintf(stderr,"Error: You must specify exactly one target process, either by name or pid.\n");
        Cmd::PrintUsage(stderr, EXIT_FAILURE);
    }
    else if(exe_ok)
    {
        settings.ExeName = exe;
    }
    else if (pid_ok)
    {
        settings.lazy = 1;
        settings.pid = pid;
    }
    else
    {
        fprintf(stderr,"Error: You must specify one target process, either by name or pid\n");
        Cmd::PrintUsage(stderr, EXIT_FAILURE);
    }

    MyExceptionHandler::SetSettings(&settings);
    MyExceptionHandler::SignalRegister();

    if( (settings.ExeName[0] || settings.pid) && (settings.nbTimeOn) && (settings.nbTimeOff >= 0) )
    {
        if(extOpenThread)
        {
            GetDebugPriv();
            if(settings.isHighPriority)
            {
                SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
            }
            if(extSetProcessWorkingSetSize)
            {
                extSetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
            }

            HANDLE prc = 0;
            DWORD curProcId = GetCurrentProcessId();

            while(! OpenMutex( MUTEX_ALL_ACCESS, 0, L"CPULimit_Deactivate_Mutex") )
            {
                if(prc && (WaitForSingleObject(prc, 0) != WAIT_TIMEOUT) )
                {
                    CloseHandle(prc);
                    prc = 0;
                }
                if(!prc)
                {
                    prc = process_finder(&settings, pid_ok);
                }

                // Control process
                if(prc)
                {
                    if(settings.pid == curProcId)
                    {
                        printf("Target process %d is cpulimit itself! Aborting because it makes no sense\n", curProcId);
                        ExitProcess(EXIT_FAILURE);
                    }
                    process_limiter(settings, prc);
                }
                else
                {
                    // Wait process
                    if (settings.lazy)
                    {
                        break;
                    }
                    Sleep(2000);
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

void process_limiter(ClSettings settings, HANDLE prc)
{
    if(settings.nbTimeOff > 0)
    {
        if(extSuspendProcess && settings.isNtDll)
        {
            extSuspendProcess(prc);
        }
        else
        {
            SuspendResumeIt(settings.pid, 1);
        }

        Sleep(settings.nbTimeOff);

        if(extResumeProcess && settings.isNtDll)
        {
            extResumeProcess(prc);
        }
        else
        {
            SuspendResumeIt(settings.pid, 0);
        }
        Sleep(settings.nbTimeOn);
    }
    else
    {
        // Always running;
        Sleep(2000);
    }
}

HANDLE process_finder(ClSettings *settings, int pid_ok)
{
    HANDLE prc = 0;
    DWORD prc_priority = NORMAL_PRIORITY_CLASS;

    if (pid_ok)
    {
        prc = ProcById(settings->pid);
    }
    else
    {
        prc = ProcByExe(settings->ExeName, settings->pid);
    }

    if(prc)
    {
        fprintf(stdout, "Process %d found.\n", settings->pid);
        switch (settings->codeExePriority)
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
