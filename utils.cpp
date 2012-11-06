#include "stdafx.h"

// Convert to a wchar_t*
void char2wc(const char *orig, wchar_t *wcstring)
{
    size_t origsize = strlen(orig) + 1;  
    size_t convertedChars = 0;  
    const size_t newsize = 10000;
	wchar_t wc[newsize] = {0};
    mbstowcs_s(&convertedChars, wc, origsize, orig, _TRUNCATE);
    wcscat_s(wc, origsize, L" (wchar_t *)");

	*wcstring = *wc;
}
