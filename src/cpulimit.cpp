/**       GNU GENERAL PUBLIC LICENSE, Version 3
 * cpulimit - CPU usage limiter for windows (cygwin)
 * Copyright (C) 2012 Alexandre Quercia <alquerci@email.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
