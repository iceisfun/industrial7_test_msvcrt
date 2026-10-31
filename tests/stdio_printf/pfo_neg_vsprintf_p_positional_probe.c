/* TC
 * id:    pfo_neg_vsprintf_p_positional_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_p positional parameters, _ARGMAX
 * note:  PROBES that _vsprintf_p rejects (returns -1, handler once): (a) mixing "%1$d %d", (b) a gap where only "%2$d" is referenced,
 *        (c) position 101 (> _ARGMAX 100), (d) position 0 ("%0$d"; positions start at 1). All three are expectations from memory of the positional-parameter rules.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap);
    return r;
}
TC_CASE(mixing)
{
    char b[32]; const char * volatile f = "%1$d %d"; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, 1, 2);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(gap)
{
    char b[32]; const char * volatile f = "%2$d"; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, 1, 2);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(position_zero)
{
    char b[32]; const char * volatile f = "%0$d"; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(beyond_argmax)
{
    char b[32]; const char * volatile f = "%101$d"; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(mixing); TC_RUN(gap); TC_RUN(beyond_argmax); TC_RUN(position_zero);
    return tc_end();
}
