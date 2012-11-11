// clsettings.cpp: check all configs.

#include "stdafx.h"

ClSettings GetSettings()
{
    ClSettings settings;

    settings.codeExePriority = 1;
    settings.ExeName = L"";
    settings.isHighPriority = 1;
    settings.isNtDll = 1;
    settings.lazy = 0;
    settings.nbTimeOff = 400;
    settings.nbTimeOn = 600;
    settings.pid = 0;

    return settings;
}
