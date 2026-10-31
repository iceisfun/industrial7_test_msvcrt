/* TC
 * id:    pfo_neg_invalid_format_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf; UCRT format-string state machine
 * note:  PROBES: each of "%" (trailing), "%l" (trailing size prefix), "%qd" (q is not a size prefix on MS CRT)
 *        yields return -1 and exactly one invalid-parameter-handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cv(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(trailing_percent)
{
    char b[32]; const char * volatile f = "abc%"; int r;
    tc_iph_count = 0;
    r = cv(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(trailing_l)
{
    char b[32]; const char * volatile f = "abc%l"; int r;
    tc_iph_count = 0;
    r = cv(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(q_prefix)
{
    char b[32]; const char * volatile f = "%qd"; int r;
    tc_iph_count = 0;
    r = cv(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(trailing_percent); TC_RUN(trailing_l); TC_RUN(q_prefix);
    return tc_end();
}
