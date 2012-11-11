#pragma once

typedef struct ClSettings ClSettings;
struct ClSettings
{
    WCHAR *ExeName;
    DWORD pid;
    int nbTimeOn;
    int nbTimeOff;
    int isHighPriority;
    int isNtDll;
    int codeExePriority;
    int lazy;
};

ClSettings GetSettings();
