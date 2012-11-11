// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#ifndef AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Tlhelp32.h>
#include <signal.h>

#include "clsettings.h"
#include "targetver.h"
#include "functions.h"
#include "RTL_calls.h"
#include "service_support.h"
#include "cpulimit.h"
#include "comandline.h"
#include "MyExceptionHandler.h"


// vendor
#include "../vendor/ermshiperete/getopt/getoptLib/getopt.h"


#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
