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
