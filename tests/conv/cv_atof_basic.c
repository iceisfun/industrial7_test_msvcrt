/* TC
 * id:    cv_atof_basic
 * area:  conv
 * funcs: atof
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.1; MS docs atof
 */
#include "tc.h"
#include <stdlib.h>

static double F(const char *volatile s) { return atof(s); }

TC_CASE(simple) { CHECK_BITS_D(F("1.5"), 1.5); CHECK_BITS_D(F("-2.25"), -2.25); CHECK_BITS_D(F("0"), 0.0); CHECK_BITS_D(F("+3"), 3.0); }
TC_CASE(exponent) { CHECK_BITS_D(F("1e3"), 1000.0); CHECK_BITS_D(F("2E+2"), 200.0); CHECK_BITS_D(F("-5e0"), -5.0); CHECK_NEAR(F("1E-2"), 0.01, 1e-17); CHECK_BITS_D(F("1E-2"), 0.01); }
TC_CASE(ws) { CHECK_BITS_D(F("  \t\n\v\f\r4.5"), 4.5); CHECK_BITS_D(F(" -0.5"), -0.5); }
TC_CASE(no_digits) { CHECK_BITS_D(F("abc"), 0.0); CHECK_BITS_D(F(""), 0.0); CHECK_BITS_D(F("."), 0.0); CHECK_BITS_D(F("e5"), 0.0); }
TC_CASE(trailing) { CHECK_BITS_D(F("3.5xyz"), 3.5); CHECK_BITS_D(F("2 3"), 2.0); CHECK_BITS_D(F("1e"), 1.0); CHECK_BITS_D(F("1e+"), 1.0); }
TC_CASE(point_forms) { CHECK_BITS_D(F(".5"), 0.5); CHECK_BITS_D(F("5."), 5.0); CHECK_BITS_D(F("-.25"), -0.25); }
TC_CASE(neg_zero) { CHECK_BITS_D(F("-0.0"), -0.0); CHECK_BITS_D(F("-0"), -0.0); CHECK_BITS_D(F("0.0"), 0.0); }

int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(exponent); TC_RUN(ws); TC_RUN(no_digits); TC_RUN(trailing); TC_RUN(point_forms); TC_RUN(neg_zero); return tc_end(); }
