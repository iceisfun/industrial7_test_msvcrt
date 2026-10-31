/* TC
 * id:    pfv_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: _TRUNCATE truncates without invoking the handler;  size < count and output too long -> handler, buffer[0]=0
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile src = "abcdef";
TC_CASE(truncate_no_iph) { char b[8]; tc_iph_count = 0; _snprintf_s(b, 4, _TRUNCATE, "%s", src); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(truncate_nul_and_ret) { char b[8]; int n; n = _snprintf_s(b, 4, _TRUNCATE, "%s", src); CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); }
TC_CASE(size_lt_count_iph) { char b[8]; int n; tc_iph_count = 0; n = _snprintf_s(b, 4, 10, "%s", src); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(buf0_after_iph) { char b[8]; int i; for (i = 0; i < 8; i++) b[i] = 'Z'; _snprintf_s(b, 4, 10, "%s", src); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(truncate_no_iph); TC_RUN(truncate_nul_and_ret); TC_RUN(size_lt_count_iph); TC_RUN(buf0_after_iph); return tc_end(); }
