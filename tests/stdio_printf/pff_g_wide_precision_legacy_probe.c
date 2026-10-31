/* TC
 * id:    pff_g_wide_precision_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   none (legacy CRT behaviour)
 * note:  PROBING: legacy CRTs generate only 17 significant digits and zero-fill the rest; %g then strips the zeros, so %.30g of 0.1 is 0.10000000000000001. Exact 0.5 is unaffected.
 */
#include "tc.h"
#include <stdio.h>

static volatile double a = 0.1, h = 0.5;

TC_CASE(tenth30) { char b[80]; sprintf(b, "%.30g", a); CHECK_STR(b, "0.10000000000000001"); }
TC_CASE(half30) { char b[80]; sprintf(b, "%.30g", h); CHECK_STR(b, "0.5"); }

int main(void) { tc_begin(); TC_RUN(tenth30); TC_RUN(half30); return tc_end(); }
