/////////////////////////////////////////////////
//////////////////////////////////service support

#include "stdafx.h"

int SvcStarted;
SERVICE_STATUS SvcStatus;
SERVICE_STATUS_HANDLE SvcHandle;
SERVICE_TABLE_ENTRY ste[2]={{L"CPULimit",&ServiceMain},{0,0}};

VOID WINAPI ServiceHandler(DWORD fdwControl)
{
    if((fdwControl == SERVICE_CONTROL_STOP) || (fdwControl == SERVICE_CONTROL_SHUTDOWN))
    {
        SvcStatus.dwCurrentState = SERVICE_STOP_PENDING; 
        SetServiceStatus(SvcHandle, &SvcStatus);
        HaltMich(); 
    }
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    SvcHandle = RegisterServiceCtrlHandler(L"CPULimit", &ServiceHandler);
    ZeroMemory(&SvcStatus, sizeof(SvcStatus));
    SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
    SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
    SvcStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(SvcHandle, &SvcStatus);
    SvcStarted = 1;
    CPULimitMain();
    SvcStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(SvcHandle, &SvcStatus);
}

VOID WINAPI ServiceStart()
{
    SvcStarted=0;
    StartServiceCtrlDispatcher(ste);
    while(!SvcStarted)Sleep(100);
}