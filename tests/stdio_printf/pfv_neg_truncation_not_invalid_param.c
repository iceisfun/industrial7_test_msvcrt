/* TC
 * id:    pfv_neg_truncation_not_invalid_param
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf _snprintf_s
 * procs: snprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs snprintf/_snprintf/_snwprintf/_snprintf_s: truncation is reported by return value only; _TRUNCATE truncation does not call the invalid parameter handler
 * note:  snprintf resolved at run time; skipped where the CRT does not export it (only ucrtbase and msvcr120 are expected to).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_DECL_PROC(int, snprintf, (char *, size_t, const char *, ...));

TC_CASE(snprintf_trunc_iph0) { char b[16]; int n; if (!tc_p_snprintf) TC_SKIP("no C99 snprintf in this CRT"); tc_iph_count = 0; n = tc_p_snprintf(b, 4, "%s", "abcdef"); CHECK_EQ_I(n, 6); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(_snprintf_trunc_iph0) { char b[16]; int n; tc_iph_count = 0; n = _snprintf(b, 4, "%s", "abcdef"); CHECK(n < 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(_snwprintf_trunc_iph0) { wchar_t b[16]; int n; tc_iph_count = 0; n = _snwprintf(b, 4, L"%ls", L"abcdef"); CHECK(n < 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(_snprintf_s_truncate_iph0) { char b[16]; int n; tc_iph_count = 0; n = _snprintf_s(b, 4, (size_t)-1, "%s", "abcdef"); CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(snprintf); CHECK(tc_iph_install()); TC_RUN(snprintf_trunc_iph0); TC_RUN(_snprintf_trunc_iph0); TC_RUN(_snwprintf_trunc_iph0); TC_RUN(_snprintf_s_truncate_iph0); return tc_end(); }
