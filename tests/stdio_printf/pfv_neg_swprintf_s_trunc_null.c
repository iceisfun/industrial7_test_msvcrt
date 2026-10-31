/* TC
 * id:    pfv_neg_swprintf_s_trunc_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swprintf_s / sprintf_s: buffer too small -> buffer[0]=0, invalid parameter handler, ERANGE; NULL args -> EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(too_small) { wchar_t b[8]; int n; b[0] = L'x'; tc_iph_count = 0; n = swprintf_s(b, 4, L"%ls", L"abcdef"); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(too_small_errno) { wchar_t b[8]; tc_iph_count = 0; errno = 0; swprintf_s(b, 4, L"%ls", L"abcdef"); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exact_fit_ok) { wchar_t b[8]; int n; tc_iph_count = 0; n = swprintf_s(b, 4, L"%ls", L"abc"); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_buf) { wchar_t *volatile p = NULL; int n; tc_iph_count = 0; errno = 0; n = swprintf_s(p, 8, L"a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { wchar_t b[8]; const wchar_t *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = swprintf_s(b, 8, f); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(too_small); TC_RUN(too_small_errno); TC_RUN(exact_fit_ok); TC_RUN(null_buf); TC_RUN(null_fmt); return tc_end(); }
