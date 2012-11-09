void __fastcall GetDebugPriv();
HANDLE __fastcall ProcByExe(char *exe, DWORD &pid);
void __fastcall SuspendResumeIt(DWORD pid, bool suspend);
void __fastcall CPULimitMain(int argc, wchar_t *argv[]);
void __fastcall HaltMich();
