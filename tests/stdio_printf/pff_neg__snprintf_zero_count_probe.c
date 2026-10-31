/* TC
 * id:    pff_neg__snprintf_zero_count_probe
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf (count 0 not specifically documented)
 * note:  probed: _snprintf (non-C99) with count 0 and a real buffer returns -1 because the output does not fit, and writes nothing.
 *        The NULL-buffer variant lives in pff_neg__snprintf_null_zero_probe (separate guess).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(zero_count) { char b[8]; volatile double v = 1.5; int n, i; for (i = 0; i < 8; i++) b[i] = 'Z';
  n = _snprintf(b, 0, "%f", v); CHECK_EQ_I(n, -1); for (i = 0; i < 8; i++) if (b[i] != 'Z') { CHECK(0); break; } }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(zero_count); return tc_end(); }
