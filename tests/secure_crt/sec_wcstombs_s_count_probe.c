/* TC
 * id:    sec_wcstombs_s_count_probe
 * area:  secure_crt
 * funcs: wcstombs_s mbstowcs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs wcstombs_s, mbstowcs_s
 * note:  PROBE: count smaller than source length and smaller than buffer size: converts exactly count chars, NUL-terminates, returns 0, *pReturnValue == count+1. Also *pReturnValue == buffer size (4) after STRUNCATE.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(wcs_count_less) { char b[8]; size_t n = 99; memset(b, 'x', sizeof b); CHECK_EQ_I(wcstombs_s(&n, b, 8, L"abcdef", 2), 0); CHECK_STR(b, "ab"); CHECK_EQ_U(n, 3); }
TC_CASE(mbs_count_less) { wchar_t b[8]; size_t n = 99; int i; for (i = 0; i < 8; i++) b[i] = L'x'; CHECK_EQ_I(mbstowcs_s(&n, b, 8, "abcdef", 3), 0); CHECK_WSTR(b, L"abc"); CHECK_EQ_U(n, 4); }
TC_CASE(wcs_truncate_retsize) { char b[4]; size_t n = 99; CHECK_EQ_I(wcstombs_s(&n, b, 4, L"abcdef", (size_t)-1), 80); CHECK_EQ_U(n, 4); }
TC_CASE(mbs_truncate_retsize) { wchar_t b[4]; size_t n = 99; CHECK_EQ_I(mbstowcs_s(&n, b, 4, "abcdef", (size_t)-1), 80); CHECK_EQ_U(n, 4); }
int main(void) { tc_begin(); TC_RUN(wcs_count_less); TC_RUN(mbs_count_less); TC_RUN(wcs_truncate_retsize); TC_RUN(mbs_truncate_retsize); return tc_end(); }
