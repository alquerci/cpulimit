#pragma once

class MyExceptionHandler
{
    private:
        static Config *settings;
        static HANDLE *handleCollection[];
        static int handleNb;

    public:
        MyExceptionHandler();
        ~MyExceptionHandler();
        static void OnExit(int dummy = 0);
        static void AddHandle(HANDLE *handle);
        static void SetSettings(Config *settings);
        static void SignalRegister();
};

