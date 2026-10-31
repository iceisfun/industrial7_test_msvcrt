/* TC
 * id:    pff_neg_f_width_overflow_probe
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf width specification
 * note:  Probes: a numeric width/precision that overflows int in the format string is rejected with -1.
 *        _scprintf only counts, so no huge allocation occurs. Invalid-parameter count not asserted.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(width_overflow)
{
    const char *volatile f = "%99999999999f"; int n;
    tc_iph_install();
    n = _scprintf(f, 1.5);
    CHECK_EQ_I(n, -1);
}
TC_CASE(precision_overflow)
{
    const char *volatile f = "%.99999999999f"; int n;
    tc_iph_install();
    n = _scprintf(f, 1.5);
    CHECK_EQ_I(n, -1);
}

int main(void) { tc_begin(); TC_RUN(width_overflow); TC_RUN(precision_overflow); return tc_end(); }
