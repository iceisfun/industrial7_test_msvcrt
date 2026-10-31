/* TC
 * id:    pff_a_msvcr120_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   VS2013 CRT C99 conformance; MS docs printf type field a
 * note:  probed: VS2013 CRT added C99 %a printf (msvcr120 prints 0x1.8p+0 and 0X1.000P+0)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static char b[64];

TC_CASE(a_lower) { volatile double d = 1.5; sprintf(b, "%.1a", d); CHECK_STR(b, "0x1.8p+0"); }
TC_CASE(A_upper) { volatile double d = 1.0; sprintf(b, "%.3A", d); CHECK_STR(b, "0X1.000P+0"); }
int main(void) { tc_begin(); TC_RUN(a_lower); TC_RUN(A_upper); return tc_end(); }
