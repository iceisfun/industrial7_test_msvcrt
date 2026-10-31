/* TC
 * id:    sec_wcstombs_s_truncate
 * area:  secure_crt
 * funcs: wcstombs_s mbstowcs_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs wcstombs_s, mbstowcs_s (_TRUNCATE, STRUNCATE)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
TC_CASE(wcstombs_truncate_returns_STRUNCATE_terminated) { char b[4]; size_t n = 99; memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, 4, L"abcdef", _TRUNCATE), STRUNCATE); CHECK_STR(b, "abc"); }
TC_CASE(mbstowcs_truncate_returns_STRUNCATE_terminated) { wchar_t b[4]; size_t n = 99; int i; for (i = 0; i < 4; i++) b[i] = L'x';
    CHECK_EQ_I(mbstowcs_s(&n, b, 4, "abcdef", _TRUNCATE), STRUNCATE); CHECK_WSTR(b, L"abc"); }
TC_CASE(truncate_fits_returns_0) { char b[8]; wchar_t w[8]; size_t n = 99;
    CHECK_EQ_I(wcstombs_s(&n, b, 8, L"abc", _TRUNCATE), 0); CHECK_STR(b, "abc"); CHECK_EQ_U(n, 4);
    n = 99; CHECK_EQ_I(mbstowcs_s(&n, w, 8, "abc", _TRUNCATE), 0); CHECK_WSTR(w, L"abc"); CHECK_EQ_U(n, 4); }
int main(void) { tc_begin(); TC_RUN(wcstombs_truncate_returns_STRUNCATE_terminated); TC_RUN(mbstowcs_truncate_returns_STRUNCATE_terminated);
    TC_RUN(truncate_fits_returns_0); return tc_end(); }
