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

ClSettings GetSettings(const WCHAR * config_path);