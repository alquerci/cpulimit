#include "stdafx.h"

ClSettings *MyExceptionHandler::settings = NULL;

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

void MyExceptionHandler::OnExit(int dummy)
{
    SuspendResumeIt(MyExceptionHandler::settings->pid, 0);
    ExitProcess(0);
}

void MyExceptionHandler::SignalRegister()
{
    signal(SIGINT, MyExceptionHandler::OnExit);
    signal(SIGTERM, MyExceptionHandler::OnExit);
    signal(SIGBREAK, MyExceptionHandler::OnExit);
    signal(SIGILL, MyExceptionHandler::OnExit);
    signal(SIGABRT, MyExceptionHandler::OnExit);
    signal(SIGABRT_COMPAT, MyExceptionHandler::OnExit);
}
