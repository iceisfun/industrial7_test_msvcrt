/* TC
 * id:    pfv_neg_fprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s fwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, fwprintf_s: NULL stream or format -> invalid parameter handler, -1, EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(fprintf_s_null_stream) { FILE *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = fprintf_s(f, "a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fprintf_s_null_fmt) { const char *volatile fm = NULL; int n; tc_iph_count = 0; errno = 0; n = fprintf_s(stdout, fm); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fwprintf_s_null_stream) { FILE *volatile f = NULL; int n; tc_iph_count = 0; errno = 0; n = fwprintf_s(f, L"a"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fwprintf_s_null_fmt) { const wchar_t *volatile fm = NULL; int n; tc_iph_count = 0; errno = 0; n = fwprintf_s(stdout, fm); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(fprintf_s_null_stream); TC_RUN(fprintf_s_null_fmt); TC_RUN(fwprintf_s_null_stream); TC_RUN(fwprintf_s_null_fmt); return tc_end(); }
