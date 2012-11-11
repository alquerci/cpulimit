#include "stdafx.h"

ClSettings *MyExceptionHandler::settings = NULL;
HANDLE *MyExceptionHandler::handleCollection[10] = {NULL};
int MyExceptionHandler::handleNb = 0;

MyExceptionHandler::MyExceptionHandler()
{
}

MyExceptionHandler::~MyExceptionHandler()
{
}

void MyExceptionHandler::SetSettings(ClSettings *settings)
{
    MyExceptionHandler::settings = settings;
}

void MyExceptionHandler::AddHandle(HANDLE *handle)
{
    MyExceptionHandler::handleCollection[MyExceptionHandler::handleNb] = handle;
    MyExceptionHandler::handleNb++;
}

void MyExceptionHandler::OnExit(int dummy)
{
    // Close all open handles
    int i = 0;

    for(i = 0 ; MyExceptionHandler::handleNb > i ; i++)
    {
        if (*MyExceptionHandler::handleCollection[i] != NULL)
        {
            CloseHandle(*MyExceptionHandler::handleCollection[i]);
            *MyExceptionHandler::handleCollection[i] = NULL;
        }
    }

    if (MyExceptionHandler::settings != NULL)
    {
        SuspendResumeIt(MyExceptionHandler::settings->pid, 0);
    }
    ExitProcess(0);
}

void MyExceptionHandler::SignalRegister()
{
    signal(SIGINT, MyExceptionHandler::OnExit);
    //signal(SIGILL, MyExceptionHandler::OnExit);
    //signal(SIGTERM, MyExceptionHandler::OnExit);
    signal(SIGBREAK, MyExceptionHandler::OnExit);
    signal(SIGABRT, MyExceptionHandler::OnExit);
    signal(SIGABRT_COMPAT, MyExceptionHandler::OnExit);
}
