/* TC
 * id:    pff_neg__snprintf_null_zero_probe
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf (count 0 not specifically documented)
 * note:  probed: _snprintf(NULL, 0, ...) is tolerated (no crash, no write) and returns -1 because the output does not fit.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_zero) { volatile double v = 1.5; char *volatile p = NULL; int n = _snprintf(p, 0, "%f", v); CHECK_EQ_I(n, -1); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_zero); return tc_end(); }
