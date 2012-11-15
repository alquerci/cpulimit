#pragma once

class Config
{
    private:
        WCHAR *m_ExeName;
        DWORD m_pid;
        int m_nbTimeOn;
        int m_nbTimeOff;
        int m_isHighPriority;
        int m_isNtDll;
        int m_codeExePriority;
        int m_isLazy;
        void GetOpt(int argc, WCHAR **argv);

    public:
        Config(int argc, WCHAR **argv);
        ~Config(void);
        void SetExeName(WCHAR * e);
        void SetProcessId(DWORD p);
        void SetTimeOn(int ton);
        void SetTimeOff(int toff);
        void SetHighPriority(int p);
        void SetNtDll(int n);
        void SetCodeExePriority(int p);
        void SetLazy(int l);
        WCHAR * GetExeName();
        DWORD GetProcessId();
        int GetTimeOn();
        int GetTimeOff();
        int GetHighPriority();
        int GetNtDll();
        int GetCodeExePriority();
        int GetLazy();
};
