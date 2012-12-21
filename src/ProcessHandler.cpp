#include "stdafx.h"
#include "ProcessHandler.h"

ProcessHandler::ProcessHandler(Config *c)
{
    this->m_cfg = c;
    this->m_error = 0;
    this->m_IsRunning = 0;

    this->m_ActivateMutex = NULL;
    this->m_Id = c->GetProcessId();
    this->m_Handle = NULL;

    this->m_extSetProcessWorkingSetSize = (extSetProcessWorkingSetSizex)GetProcAddress(LoadLibrary(L"kernel32.dll"), "SetProcessWorkingSetSize");
    this->m_extOpenThread = (extOpenThreadx)GetProcAddress(LoadLibrary(L"kernel32.dll"), "OpenThread");
    this->m_extResumeProcess = (extResumeProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtResumeProcess");
    this->m_extSuspendProcess = (extSuspendProcessx)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtSuspendProcess");

    this->FindHandle();

    this->m_aMutexName = NULL;
    this->m_dMutexName = NULL;
    this->m_aMutexName = this->GenerateMutexName(1);
    this->m_dMutexName = this->GenerateMutexName(0);

    if( OpenMutex(MUTEX_ALL_ACCESS, 0, this->m_aMutexName) )
    {
        if(this->m_cfg->GetClose())
        {
            if(CreateMutex(0, true, this->m_dMutexName) != NULL)
            {
                if(this->m_cfg->GetProcessId())
                {
                    fwprintf(stdout, L"Close the cpulimit attached by the process id: %d.\n", this->m_cfg->GetProcessId());
                }
                else
                {
                    fwprintf(stdout, L"Close the cpulimit attached by the process name: %s.\n", this->m_cfg->GetExeName());
                }
                Sleep(Config::TIME_SLOT*2);
            }
        }
        else
        {
            fwprintf(stderr, L"cpulimit already started!\n");
        }
        this->m_error += 1;
    }
    else if (this->m_cfg->GetClose())
    {
        fwprintf(stderr, L"No process exists with MUTEX: %s. Exiting...\n", this->m_aMutexName);
        this->m_error += 1;
    }
    else
    {
        this->m_ActivateMutex = CreateMutex(0, true, this->m_aMutexName);
    }

    if (!this->m_extOpenThread)
    {
        fwprintf(stderr,L"Your system is not supported. Exiting.\n");
        this->m_error += 1;
    }

    if(!this->m_error)
    {
        if(this->m_extSetProcessWorkingSetSize)
        {
            this->m_extSetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T) -1, (SIZE_T) -1);
        }

        if(this->m_cfg->GetHighPriority())
        {
            if(SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
            {
                if(this->m_cfg->GetVerbose()) fprintf(stdout, "Priority changed to %d for cpulimit.\n", HIGH_PRIORITY_CLASS);
            }
            else
            {
                fprintf(stderr, "Failed to set priority to %d\n", HIGH_PRIORITY_CLASS);
            }
        }

        this->GetDebugPrivilege();
    }
}

ProcessHandler::~ProcessHandler(void)
{
    if(this->m_ActivateMutex)
    {
        CloseHandle(this->m_ActivateMutex);
    }
    if(this->m_Handle)
    {
        CloseHandle(this->m_Handle);
    }

    free((void*)this->m_aMutexName);
    free((void*)this->m_dMutexName);
}

int ProcessHandler::GetError()
{
    return this->m_error;
}

int ProcessHandler::CheckState()
{
    int state = 0;
    if(this->m_Id + this->m_cfg->GetExeName() > 0)
    {
        if ( ! OpenMutex( MUTEX_ALL_ACCESS, 0, this->m_dMutexName ) )
        {
            state = 1;
        }
    }

    if(this->m_Handle)
    {
        if(WaitForSingleObject(this->m_Handle, 0) != WAIT_TIMEOUT)
        {
            fprintf(stdout, "Process %d closed.\n", this->m_Id);
            CloseHandle(this->m_Handle);
            this->m_Handle = NULL;
            this->m_Id = NULL;
            this->m_IsRunning = 0;
            if(this->m_cfg->GetLazy())
            {
                return 0;
            }
        }
    }

    if(!this->m_Handle)
    {
        this->FindHandle();
    }

    if(this->m_Id == GetCurrentProcessId())
    {
        printf("Target process %d is cpulimit itself!\nAborting because it makes no sense.\n", this->m_Id);
        state = 0;
    }

    if(MyExceptionHandler::GetIsExiting())
    {
        state = 0;
    }

    return state;
}

void ProcessHandler::Resume()
{
    DWORD error = 0;
    if(this->m_extResumeProcess && this->m_cfg->GetNtDll())
    {
        error = this->m_extResumeProcess(this->m_Handle);
        if(error)
        {
            SuspendResumeIt(this->m_Id, 0);
        }
    }
    else
    {
        SuspendResumeIt(this->m_Id, 0);
    }
    this->m_IsRunning = 1;
}

void ProcessHandler::Suspend()
{
    DWORD error = 0;
    if(this->m_extSuspendProcess && this->m_cfg->GetNtDll())
    {
        this->m_extSuspendProcess(this->m_Handle);
        if(error)
        {
            SuspendResumeIt(this->m_Id, 1);
        }
    }
    else
    {
        SuspendResumeIt(this->m_Id, 1);
    }
    this->m_IsRunning = 0;
}

HANDLE ProcessHandler::FindHandle()
{
    if (this->m_Id)
    {
        this->m_Handle = OpenProcess(PROCESS_SUSPEND_RESUME, 0, this->m_Id);
    }
    else
    {
        this->m_Handle = this->FindHandleByName(this->m_cfg->GetExeName());
    }

    if(this->m_Handle)
    {
        fprintf(stdout, "Process %d found.\n", this->m_Id);
        SetPriorityClass(this->m_Handle, this->m_cfg->GetCodeExePriority());
        if(this->m_cfg->GetVerbose())
        {
            fprintf(stdout, "Priority changed to %d for the process %d.\n", this->m_cfg->GetCodeExePriority(), this->m_Id);
        }
        this->m_IsRunning = 1;
    }
    else
    {
        this->m_IsRunning = 0;
    }

    return this->m_Handle;
}

void ProcessHandler::GetDebugPrivilege()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
}

void __fastcall ProcessHandler::SuspendResumeIt(DWORD pid, bool suspend)
{
    HANDLE snp = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    HANDLE trd = NULL;
    if(snp != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 pe = {sizeof(THREADENTRY32), 0};
        if(Thread32First(snp, &pe))
        {
            do
            {
                if(pe.th32OwnerProcessID == pid)
                {
                    trd = this->m_extOpenThread(THREAD_SUSPEND_RESUME, 0, pe.th32ThreadID);
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

HANDLE ProcessHandler::FindHandleByName(WCHAR *exe)
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

                if( ! wcscmp(t, exe) )
                {
                    prc = OpenProcess(PROCESS_SUSPEND_RESUME, 0, pe.th32ProcessID);
                    this->m_Id = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(snp, &pe));
        }
        CloseHandle(snp);
        snp = NULL;
    }
    return prc;
}

int ProcessHandler::IsOpen()
{
    if (this->m_Handle != NULL)
    {
        return 1;
    }
    return 0;
}

int ProcessHandler::IsRunning()
{
    return this->m_IsRunning;
}

WCHAR * ProcessHandler::GenerateMutexName(int mode)
{
    WCHAR wcs[250] = {NULL};
    WCHAR *out = NULL;
    size_t name_size = 0;
    
    if(this->m_Id)
    {
        wsprintf(wcs, L"cpulimit_MODE-%d_PROCESSID-%d", mode, this->m_Id);
    }
    else if (this->m_cfg->GetExeName())
    {
        wsprintf(wcs, L"cpulimit_MODE-%d_PROCESSNAME-%s", mode, this->m_cfg->GetExeName());
    }
    else
    {
        wsprintf(wcs, L"cpulimit_MODE-%d", mode);
    }

    name_size = wcslen(wcs)+1;

    out = (WCHAR*) calloc(name_size, sizeof(WCHAR));

    wcscpy_s(out, name_size, wcs);

    return out;
}
