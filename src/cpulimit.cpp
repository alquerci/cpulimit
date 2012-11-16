// cpulimit.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"


int wmain(int argc, WCHAR *argv[])
{
    Config *settings = new Config(argc, argv);
    if(settings->GetError())
    {
        //Config::PrintUsage(stderr);
        delete settings;
        ExitProcess(EXIT_FAILURE);
    }
    MyExceptionHandler::SetConfig(settings);


    ProcessHandler *ph = new ProcessHandler(settings);
    if(ph->GetError())
    {
        delete ph;
        delete settings;
        ExitProcess(EXIT_FAILURE);
    }
    MyExceptionHandler::SetProcessHandler(ph);

    MyExceptionHandler::SignalRegister();
    while(ph->CheckState())
    {
        // Control process
        if(ph->IsOpen())
        {
            if(settings->GetTimeOff() > 0)
            {
                ph->Suspend();
                Sleep(settings->GetTimeOff());
                ph->Resume();
                Sleep(settings->GetTimeOn());
            }
            else
            {
                Sleep(Config::TIME_SLOT);
            }
        }
        else
        {
            // Wait process
            if (settings->GetLazy())
            {
                break;
            }
            Sleep(Config::TIME_SLOT * 2);
        }
    }

    delete ph;
    delete settings;
    ExitProcess(EXIT_SUCCESS);
}
