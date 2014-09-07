/**       GNU GENERAL PUBLIC LICENSE, Version 3
 * cpulimit - CPU usage limiter for windows (cygwin)
 * Copyright (C) 2012 Alexandre Quercia <alquerci@email.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#ifndef AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// #define UNICODE
// #define _UNICODE

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Tlhelp32.h>
#include <signal.h>

#if HAVE_CONFIG_H
    #include "../config.h"
#endif

#ifndef VERSION
    #define VERSION "2.1.0-DEV"
#endif

#include "targetver.h"
#include "Config.h"
#include "ProcessHandler.h"
#include "MyExceptionHandler.h"
#include "cpulimit.h"

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
