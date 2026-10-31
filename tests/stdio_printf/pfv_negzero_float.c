/* TC
 * id:    pfv_negzero_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 (sign of negative zero is output)

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double nz = -0.0, pz = 0.0;
TC_CASE(f) { char b[64]; sprintf(b, "%f", nz); CHECK_STR(b, "-0.000000"); }
TC_CASE(g) { char b[64]; sprintf(b, "%g", nz); CHECK_STR(b, "-0"); }
TC_CASE(f0) { char b[64]; sprintf(b, "%.0f", nz); CHECK_STR(b, "-0"); }
TC_CASE(plus_zero) { char b[64]; sprintf(b, "%+f", pz); CHECK_STR(b, "+0.000000"); }

int main(void) { tc_begin(); TC_RUN(f); TC_RUN(g); TC_RUN(f0); TC_RUN(plus_zero); return tc_end(); }
