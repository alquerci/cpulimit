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

class Config
{
    private:
        int m_error;
        char *m_ExeName;
        DWORD m_pid;
        int m_nbTimeOn;
        int m_nbTimeOff;
        int m_isHighPriority;
        int m_isNtDll;
        int m_codeExePriority;
        int m_isLazy;
        int m_Close;
        int m_Verbose;
        int m_CpuUsage;

        void GetOpt(int argc, char **argv);
        void SetExeName(char * e);
        void SetProcessId(DWORD p);
        void SetLimit(int l);
        void SetHighPriority(int p);
        void SetNtDll(int n);
        void SetCodeExePriority(int p);
        void SetLazy(int l);
        void SetClose(int close);
        void SetVerbose(int v);

    public:
        // TIME_SLOT must be ending by 00 (ex: 100, 200, ..., 1100, ...)
        static const unsigned int TIME_SLOT;

        Config(int argc, char **argv);
        ~Config(void);

        int GetError(void);
        static void PrintUsage(FILE * stream);
        char * GetExeName();
        DWORD GetProcessId();
        int GetTimeOn();
        int GetTimeOff();
        int GetHighPriority();
        int GetNtDll();
        int GetCodeExePriority();
        int GetLazy();
        int GetClose();
        int GetVerbose();
};
