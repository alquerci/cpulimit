#pragma once

typedef struct ClSettings ClSettings;
struct ClSettings
{
    WCHAR ExeName[261];
    int nbTimeOn;
    int nbTimeOff;
    int isHighPriority;
    int isNtDll;
    int codeExePriority;
};

WCHAR * CreateConfigPath(const WCHAR * program_path);
ClSettings GetSettings(const WCHAR * config_path);