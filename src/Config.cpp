#include "stdafx.h"

Config::Config(int argc, WCHAR **argv)
{
    this->m_codeExePriority = 1;
    this->m_ExeName = NULL;
    this->m_isHighPriority = 1;
    this->m_isNtDll = 1;
    this->m_isLazy = 0;
    this->m_nbTimeOff = 0;
    this->m_nbTimeOn = 0;
    this->m_pid = 0;

    this->GetOpt(argc, argv);
}

Config::~Config(void)
{
}

void Config::GetOpt(int argc, WCHAR **argv)
{
    //argument variables
    int exe_ok = 0;
    int pid_ok = 0;

    WCHAR *exe = NULL;
    int pid = 0;

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
                this->SetLimit(_wtoi(optarg));
                break;
            case 'z':
                this->SetLazy(1);
                break;
            case 'I':
                this->SetCodeExePriority(0);
                break;
            case 'h':
                Cmd::PrintUsage(stdout, EXIT_SUCCESS);
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

    if (exe_ok && pid_ok)
    {
        fprintf(stderr, "Error: You must specify exactly one target process, either by name or pid.\n");
        Cmd::PrintUsage(stderr, EXIT_FAILURE);
    }
    else if (exe_ok)
    {
        this->SetExeName(exe);
    }
    else if (pid_ok)
    {
        this->SetLazy(1);
        this->SetProcessId(pid);
    }
    else
    {
        fprintf(stderr, "Error: You must specify one target process, either by name or pid.\n");
        Cmd::PrintUsage(stderr, EXIT_FAILURE);
    }

}

void Config::SetExeName(WCHAR * e)
{
    this->m_ExeName = e;
}

void Config::SetProcessId(DWORD p)
{
    this->m_pid = p;
}

void Config::SetLimit(int l)
{
    if (l < 1 || l > 100)
    {
        fprintf(stderr,"Error: Limit must be in the range 1-100.\n");
        Cmd::PrintUsage(stderr, EXIT_FAILURE);
    }
    else
    {
        this->m_nbTimeOn = l * 10;
        this->m_nbTimeOff = 1000 - this->GetTimeOn();
    }
}

void Config::SetHighPriority(int p)
{
    this->m_isHighPriority = p;
}

void Config::SetNtDll(int n)
{
    this->m_isNtDll = n;
}

void Config::SetCodeExePriority(int p)
{
    this->m_codeExePriority = p;
}

void Config::SetLazy(int l)
{
    this->m_isLazy = l;
}



WCHAR * Config::GetExeName()
{
    return this->m_ExeName;
}

DWORD Config::GetProcessId()
{
    return this->m_pid;
}

int Config::GetTimeOn()
{
    return this->m_nbTimeOn;
}

int Config::GetTimeOff()
{
    return this->m_nbTimeOff;
}

int Config::GetHighPriority()
{
    return this->m_isHighPriority;
}

int Config::GetNtDll()
{
    return this->m_isNtDll;
}

int Config::GetCodeExePriority()
{
    return this->m_codeExePriority;
}

int Config::GetLazy()
{
    return this->m_isLazy;
}
