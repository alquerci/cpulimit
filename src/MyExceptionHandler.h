#pragma once

class MyExceptionHandler
{
    private:
        static ClSettings *settings;

    public:
        MyExceptionHandler();
        ~MyExceptionHandler();
        static void OnExit(int dummy=0);
        static void SetPrc(HANDLE prc);
        static void SetSettings(ClSettings *settings);
};

