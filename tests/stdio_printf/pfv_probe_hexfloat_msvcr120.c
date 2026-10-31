/* TC
 * id:    pfv_probe_hexfloat_msvcr120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field a/A (VS2013 C99 support)
 * note:  PROBE: VS2013 added C99 %a. Asserts %.1a 1.0 -> 0x1.0p+0 and %.2A 255.0 -> 0X1.FEP+7 (255 = 1.1111111b * 2^7). Possible real difference: 3-digit exponent (p+000); if so the real output is the finding.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

TC_CASE(one_prec1) { char b[64]; volatile double v = 1.0; sprintf(b, "%.1a", v); CHECK_STR(b, "0x1.0p+0"); }
TC_CASE(upper_prec2) { char b[64]; volatile double v = 255.0; sprintf(b, "%.2A", v); CHECK_STR(b, "0X1.FEP+7"); }

int main(void) { tc_begin(); TC_RUN(one_prec1); TC_RUN(upper_prec2); return tc_end(); }
