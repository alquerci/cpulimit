// some RTL calls substitutions

#include "stdafx.h"

wchar_t* __fastcall mystrstr(const wchar_t *str1, const wchar_t *str2)
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
            return (wchar_t*) &str1[(i1 - wcslen(str2))];
        }

        i1++;
    }

    return NULL;
}

int __fastcall mystricmp(WCHAR *s1, WCHAR *s2)
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
