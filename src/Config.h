#pragma once
#define EXPORTS_GETOPT

class Config
{
    private:
        int m_error;
        WCHAR *m_ExeName;
        DWORD m_pid;
        int m_nbTimeOn;
        int m_nbTimeOff;
        int m_isHighPriority;
        int m_isNtDll;
        int m_codeExePriority;
        int m_isLazy;
        int m_Close;
        int m_Verbose;
        int m_CpuUsage;

        void GetOpt(int argc, WCHAR **argv);
        void SetExeName(WCHAR * e);
        void SetProcessId(DWORD p);
        void SetLimit(int l);
        void SetHighPriority(int p);
        void SetNtDll(int n);
        void SetCodeExePriority(int p);
        void SetLazy(int l);
        void SetClose(int close);
        void SetVerbose(int v);

    public:
        // TIME_SLOT must be ending by 00 (ex: 100, 200, ..., 1100, ...)
        static const unsigned int TIME_SLOT;

        Config(int argc, WCHAR **argv);
        ~Config(void);

        int GetError(void);
        static void PrintUsage(FILE * stream);
        WCHAR * GetExeName();
        DWORD GetProcessId();
        int GetTimeOn();
        int GetTimeOff();
        int GetHighPriority();
        int GetNtDll();
        int GetCodeExePriority();
        int GetLazy();
        int GetClose();
        int GetVerbose();
};
