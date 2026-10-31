/* TC
 * id:    pff_a_denormal_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1 a conversion; MS docs printf type field a
 * note:  probed: ONLY the denormal case; denormals printed with leading 0 digit and p-1022 (not normalised)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

TC_CASE(denorm_min) { volatile double d = dfb(1ULL); sprintf(b, "%.13a", d); CHECK_STR(b, "0x0.0000000000001p-1022"); }
int main(void) { tc_begin(); TC_RUN(denorm_min); return tc_end(); }
