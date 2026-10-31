/* TC
 * id:    sec_neg_wcstombs_s_range
 * area:  secure_crt
 * funcs: wcstombs_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s (ERANGE)
 * note:  
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
static volatile size_t v_sz;
TC_CASE(too_small_ERANGE) { char b[4]; size_t n = 99; tc_iph_install(); v_sz = sizeof b; memset(b, 'x', sizeof b);
    CHECK_EQ_I(wcstombs_s(&n, b, v_sz, L"abcdefgh", 8), ERANGE); }
TC_CASE(errno_ERANGE) { char b[4]; size_t n = 99; tc_iph_install(); v_sz = sizeof b; errno = 0;
    wcstombs_s(&n, b, v_sz, L"abcdefgh", 8); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_called) { char b[4]; size_t n = 99; tc_iph_install(); tc_iph_count = 0; v_sz = sizeof b;
    wcstombs_s(&n, b, v_sz, L"abcdefgh", 8); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(too_small_ERANGE); TC_RUN(errno_ERANGE); TC_RUN(handler_called); return tc_end(); }
