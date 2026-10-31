/* TC
 * id:    pff_f_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 f; MS docs printf type field f
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(simple) { TF("%f", 1.5, "1.500000"); TF("%f", 0.0, "0.000000"); TF("%f", -2.25, "-2.250000"); }
TC_CASE(prec3) { TF("%.3f", 123456.789, "123456.789"); }
TC_CASE(big) { TF("%f", 1e10, "10000000000.000000"); }
TC_CASE(carry) { TF("%.1f", 99.96, "100.0"); TF("%.2f", 9.999, "10.00"); }
TC_CASE(retval) { char b[64]; volatile double v = -12.5; int n = sprintf(b, "%f", v); CHECK_EQ_I(n, (int)strlen(b)); CHECK_EQ_I(n, 10); }

int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(prec3); TC_RUN(big); TC_RUN(carry); TC_RUN(retval); return tc_end(); }
