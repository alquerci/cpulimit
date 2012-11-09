// clsettings.cpp: check all configs.

#include "stdafx.h"

ClSettings GetSettings(const WCHAR * config_path)
{
    ClSettings settings = {L"", 0, 0, 0, 0, 0};

    GetPrivateProfileString(L"Settings", L"ExeName", L"", settings.ExeName, 260, config_path);
    settings.nbTimeOn = GetPrivateProfileInt(L"Settings", L"TimeOn", 1000, config_path);
    settings.nbTimeOff = GetPrivateProfileInt(L"Settings", L"TimeOff", 1000, config_path);
    settings.isHighPriority = GetPrivateProfileInt(L"Settings", L"HighPriority", 0, config_path);
    settings.isNtDll = GetPrivateProfileInt(L"Settings", L"CanUseNtDll", 1, config_path);
    settings.codeExePriority = GetPrivateProfileInt(L"Settings", L"ExePriority", 1, config_path);

    return settings;
}
