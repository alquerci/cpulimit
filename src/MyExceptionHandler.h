#pragma once

class MyExceptionHandler
{
    private:
        static ProcessHandler *ph;
        static Config *cfg;
        static int IsExiting;

    public:
        MyExceptionHandler(void);
        ~MyExceptionHandler(void);
        static BOOL CtrlHandler(DWORD dwCtrlType);
        static void SetProcessHandler(ProcessHandler *ph);
        static void SetConfig(Config *cfg);
        static void SignalRegister(void);
        static int GetIsExiting(void);
};

