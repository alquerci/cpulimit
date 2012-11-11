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
