/* TC
 * id:    pff_f_negzero
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (sign of a negative zero is printed); IEEE 754
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static volatile double nz_src = 0.0;
static double negzero(void) { double z = nz_src; return -z; }

TC_CASE(f) { TF("%f", negzero(), "-0.000000"); TF("%.0f", negzero(), "-0"); }
TC_CASE(plus) { TF("%+f", 0.0, "+0.000000"); TF("%+f", negzero(), "-0.000000"); }
TC_CASE(e)
{   /* exponent width differs between CRT generations (e+00 vs e+000): check mantissa and sign only */
    char b[64]; volatile double v = negzero(); int n = sprintf(b, "%e", v), i;
    CHECK(strncmp(b, "-0.000000e+0", 12) == 0);
    for (i = 11; b[i]; i++) CHECK(b[i] == '0' || i == 11);
    CHECK_EQ_I(n, (int)strlen(b));
}
TC_CASE(g) { TF("%g", negzero(), "-0"); TF("%g", 0.0, "0"); }

int main(void) { tc_begin(); TC_RUN(f); TC_RUN(plus); TC_RUN(e); TC_RUN(g); return tc_end(); }
