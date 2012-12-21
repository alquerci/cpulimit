#pragma once

typedef DWORD (WINAPI *extSetProcessWorkingSetSizex)(HANDLE, SIZE_T, SIZE_T);
typedef HANDLE (WINAPI *extOpenThreadx)(DWORD, BOOL, DWORD);
typedef DWORD (WINAPI *extResumeProcessx)(HANDLE);
typedef DWORD (WINAPI *extSuspendProcessx)(HANDLE);

class ProcessHandler
{
    private:
        int m_error;
        int m_IsRunning;
        extSetProcessWorkingSetSizex m_extSetProcessWorkingSetSize;
        extOpenThreadx m_extOpenThread;
        extResumeProcessx m_extResumeProcess;
        extSuspendProcessx m_extSuspendProcess;
        DWORD m_Id;
        HANDLE m_Handle;
        HANDLE m_ActivateMutex;
        WCHAR* m_aMutexName;
        WCHAR* m_dMutexName;
        Config * m_cfg;

        void __fastcall GetDebugPrivilege();
        void __fastcall SuspendResumeIt(DWORD pid, bool suspend);
        HANDLE FindHandleByName(WCHAR *exe);
        WCHAR * GenerateMutexName(int mode);

    public:
        ProcessHandler(Config *c);
        ~ProcessHandler(void);
        int GetError();
        HANDLE FindHandle();
        /**
         * return 1 all are OK
         */
        int CheckState();
        int IsOpen();
        int IsRunning();
        void Resume();
        void Suspend();
};
