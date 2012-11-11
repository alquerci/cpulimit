#pragma once

WCHAR* __fastcall mystrstr(const WCHAR *str1, const WCHAR *str2);
int __fastcall mystricmp(WCHAR *s1, const WCHAR *s2);

#ifndef _WSTDLIB_DEFINED
    int _wtoi(const wchar_t * _Str);
#endif
