/* TC
 * id:    pff_a_extremes_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.21.6.1 a conversion (explicit precision 13 fixes digits); MS docs printf type field a
 * note:  smallest normal and largest finite double with %.13a; no denormal involved
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

TC_CASE(dbl_min) { volatile double d = dfb(0x0010000000000000ULL); sprintf(b, "%.13a", d); CHECK_STR(b, "0x1.0000000000000p-1022"); }
TC_CASE(dbl_max) { volatile double d = dfb(0x7FEFFFFFFFFFFFFFULL); sprintf(b, "%.13a", d); CHECK_STR(b, "0x1.fffffffffffffp+1023"); }
int main(void) { tc_begin(); TC_RUN(dbl_min); TC_RUN(dbl_max); return tc_end(); }
