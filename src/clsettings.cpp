// clsettings.cpp: check all configs.

#include "stdafx.h"


WCHAR * CreateConfigPath(const WCHAR *program_path)
{
    rsize_t path_size = 0;
    path_size = wcslen(program_path);

    WCHAR *tmp = NULL;
    int nbcharexe = 0;

    tmp = (WCHAR *) malloc(sizeof(WCHAR) * (path_size+1));

    wcscpy_s(tmp, path_size+1, program_path);
    tmp = &tmp[path_size];

    while(*tmp != '\\' && *tmp != '/')
    {
        nbcharexe++;
        tmp--;
    }
    tmp++;
    nbcharexe--;

    wcscpy_s(tmp, wcslen(L"cpulimit.ini")+1, L"cpulimit.ini");

    return tmp-(path_size-nbcharexe);
}


ClSettings GetSettings(const WCHAR * config_path)
{
    ClSettings settings = {L"", 0, 0, 0, 0, 0};

    GetPrivateProfileString(L"Settings", L"ExeName", L"", settings.ExeName, 260, config_path);
    settings.nbTimeOn = GetPrivateProfileInt(L"Settings", L"TimeOn", 1000, config_path);
    settings.nbTimeOff = GetPrivateProfileInt(L"Settings", L"TimeOff", 1000, config_path);
    settings.isHighPriority = GetPrivateProfileInt(L"Settings", L"HighPriority", 0, config_path);
    settings.isNtDll = GetPrivateProfileInt(L"Settings", L"CanUseNtDll", 1, config_path);
    settings.codeExePriority = GetPrivateProfileInt(L"Settings", L"ExePriority", 1, config_path);

    free((void*) config_path);

    return settings;
}
