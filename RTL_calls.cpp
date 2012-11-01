// some RTL calls substitutions

#include "stdafx.h"

wchar_t *__fastcall mystrstr(wchar_t *str1, wchar_t *str2)
{
	if( (!str1) || (!str2) || (!*str1) || (!*str2) )
	{	
		return 0;
	}
	
	wchar_t *t1 = str1;

	while(*t1)
	{
		wchar_t *z1 = t1, *z2 = str2;
		
		while( *z1 && *z2 && (*z1 == *z2) )
		{
			z1++;
			z2++;
		}

		if(!*z2)
		{
			return t1;
		}

		t1++;
	 }
	return 0;
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
