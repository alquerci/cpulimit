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

// some RTL calls substitutions

#include "stdafx.h"

WCHAR* __fastcall mystrstr(const WCHAR *str1, const WCHAR *str2)
{
    if( (!str1) || (!str2) || (!*str1) || (!*str2) )
    {	
        return 0;
    }

    int i1 = 0, i2 = 0;


    while(str1[i1] != '\0')
    {
        while( str1[i1] != '\0' && str2[i2] != '\0' && (str1[i1] == str2[i2]) )
        {
            i1++;
            i2++;
        }

        if(str2[i2] == '\0')
        {
            return (WCHAR*) &str1[(i1 - wcslen(str2))];
        }

        i1++;
    }

    return NULL;
}

int __fastcall mystricmp(WCHAR *s1, const WCHAR *s2)
{
    switch( CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, s1, -1, s2, -1) )
    {
    case CSTR_LESS_THAN: 
        return -1;
    case CSTR_GREATER_THAN: 
        return 1;
    case CSTR_EQUAL: 
        return 0;
    }
    return 2;
}
