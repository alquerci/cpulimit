#pragma once

class MyExceptionHandler
{
    private:
        static ClSettings *settings;
        static HANDLE *handleCollection[];
        static int handleNb;

    public:
        MyExceptionHandler();
        ~MyExceptionHandler();
        static void OnExit(int dummy = 0);
        static void AddHandle(HANDLE *handle);
        static void SetSettings(ClSettings *settings);
        static void SignalRegister();
};

