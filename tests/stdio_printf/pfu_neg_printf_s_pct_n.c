/* TC
 * id:    pfu_neg_printf_s_pct_n
 * area:  stdio_printf
 * kind:  negative
 * funcs: printf_s vprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf, printf_s, _set_printf_count_output (%n is disabled by default: invalid parameter handler, -1)
 * note:  the pointed-to int must stay untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vp(const char *fmt, ...) { va_list ap; int r; va_start(ap, fmt); r = vprintf_s(fmt, ap); va_end(ap); return r; }

TC_CASE(printf_s_pct_n) { int n = 0x1234; int r; tc_iph_count = 0; r = printf_s("%n", &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x1234); }
TC_CASE(vprintf_s_pct_n) { int n = 0x1234; int r; tc_iph_count = 0; r = vp("%n", &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0x1234); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(printf_s_pct_n); TC_RUN(vprintf_s_pct_n);
    fflush(stdout);
    return tc_end();
}
