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

#pragma once

class MyExceptionHandler
{
    private:
        static ClSettings *settings;
        static HANDLE *handleCollection[];
        static int handleNb;

    public:
        MyExceptionHandler();
        ~MyExceptionHandler();
        static void OnExit(int dummy = 0);
        static void AddHandle(HANDLE *handle);
        static void SetSettings(ClSettings *settings);
        static void SignalRegister();
};

