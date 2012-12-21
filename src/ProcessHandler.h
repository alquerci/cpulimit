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

typedef DWORD (WINAPI *extSetProcessWorkingSetSizex)(HANDLE, SIZE_T, SIZE_T);
typedef HANDLE (WINAPI *extOpenThreadx)(DWORD, BOOL, DWORD);
typedef DWORD (WINAPI *extResumeProcessx)(HANDLE);
typedef DWORD (WINAPI *extSuspendProcessx)(HANDLE);

class ProcessHandler
{
    private:
        int m_error;
        int m_IsRunning;
        extSetProcessWorkingSetSizex m_extSetProcessWorkingSetSize;
        extOpenThreadx m_extOpenThread;
        extResumeProcessx m_extResumeProcess;
        extSuspendProcessx m_extSuspendProcess;
        DWORD m_Id;
        HANDLE m_Handle;
        HANDLE m_ActivateMutex;
        WCHAR* m_aMutexName;
        WCHAR* m_dMutexName;
        Config * m_cfg;

        void __fastcall GetDebugPrivilege();
        void __fastcall SuspendResumeIt(DWORD pid, bool suspend);
        HANDLE FindHandleByName(WCHAR *exe);
        WCHAR * GenerateMutexName(int mode);

    public:
        ProcessHandler(Config *c);
        ~ProcessHandler(void);
        int GetError();
        HANDLE FindHandle();
        /**
         * return 1 all are OK
         */
        int CheckState();
        int IsOpen();
        int IsRunning();
        void Resume();
        void Suspend();
};
