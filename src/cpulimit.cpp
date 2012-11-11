// cpulimit.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"


int wmain(int argc, WCHAR *argv[])
{
    MyExceptionHandler::SignalRegister();

    cpulimitMain(argc, argv);

    ExitProcess(0);
}
