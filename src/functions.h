void __fastcall GetDebugPriv();
HANDLE __fastcall ProcByExe(char *exe, DWORD &pid);
void __fastcall SuspendResumeIt(DWORD pid, bool suspend);
void __fastcall CPULimitMain(int argc, WCHAR *argv[]);
void __fastcall HaltMich();
WCHAR * CreateConfigPath(const WCHAR * program_path);
