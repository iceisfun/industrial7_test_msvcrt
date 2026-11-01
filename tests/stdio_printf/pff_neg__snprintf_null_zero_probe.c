/* TC
 * id:    pff_neg__snprintf_null_zero_probe
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _snprintf (count 0 not specifically documented)
 * note:  Observed on Windows 10 22H2: _snprintf(NULL, 0, "%f", 1.5) is tolerated (no crash, no handler call, errno 0) and returns 8, the
 *        would-be length (C99 snprintf style), not -1.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_zero) { volatile double v = 1.5; char *volatile p = NULL; int n = _snprintf(p, 0, "%f", v); CHECK_EQ_I(n, 8); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(null_zero); return tc_end(); }
