#include "stdafx.h"
#include "MyExceptionHandler.h"

ProcessHandler *MyExceptionHandler::ph = NULL;
Config *MyExceptionHandler::cfg = NULL;
int MyExceptionHandler::IsExiting = 0;

MyExceptionHandler::MyExceptionHandler()
{
}

MyExceptionHandler::~MyExceptionHandler()
{
}

void MyExceptionHandler::SetProcessHandler(ProcessHandler *ph)
{
    MyExceptionHandler::ph = ph;
}

void MyExceptionHandler::SetConfig(Config *cfg)
{
    MyExceptionHandler::cfg = cfg;
}

BOOL MyExceptionHandler::CtrlHandler(DWORD dwCtrlType)
{
    if (MyExceptionHandler::ph != NULL)
    {
        if (ph->IsOpen() && !ph->IsRunning())
        {
            MyExceptionHandler::ph->Resume();
        }
    }

    if(dwCtrlType != CTRL_BREAK_EVENT)
    {
        MyExceptionHandler::IsExiting = 1;
    }

    return TRUE;
}

void MyExceptionHandler::SignalRegister()
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)MyExceptionHandler::CtrlHandler, TRUE);
}

int MyExceptionHandler::GetIsExiting()
{
    return IsExiting;
}
