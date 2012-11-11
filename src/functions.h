#pragma once

void __fastcall GetDebugPriv();
HANDLE __fastcall ProcByExe(WCHAR *exe, DWORD &pid);
HANDLE __fastcall ProcById(DWORD pid);
void __fastcall SuspendResumeIt(DWORD pid, bool suspend);
void __fastcall cpulimitMain(int argc, WCHAR *argv[]);

void process_limiter(ClSettings settings, HANDLE prc);
HANDLE process_finder(ClSettings *settings, int pid_ok);

typedef DWORD (WINAPI *extSetProcessWorkingSetSizex)(HANDLE, int,int);
typedef HANDLE (WINAPI *extOpenThreadx)(DWORD, BOOL, DWORD);
typedef DWORD (WINAPI *extResumeProcessx)(HANDLE);
typedef DWORD (WINAPI *extSuspendProcessx)(HANDLE);

