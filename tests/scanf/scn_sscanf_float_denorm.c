/* TC
 * id:    scn_sscanf_float_denorm
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3, 5.2.4.2.2, IEEE 754 correct rounding
 * note:  ucrt only: older CRTs are not guaranteed to round correctly or handle denormals exactly
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(min_denorm) { double d = 0; int n = sscanf("4.9406564584124654e-324", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 1ull); }
TC_CASE(dbl_min) { double d = 0; int n = sscanf("2.2250738585072014e-308", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0x0010000000000000ull); }
TC_CASE(dbl_max) { double d = 0; int n = sscanf("1.7976931348623157e308", "%lf", &d); CHECK_EQ_I(n, 1); CHECK(tc_dbits(d) == 0x7fefffffffffffffull); }
TC_CASE(flt_min) { float f = 0; int n = sscanf("1.17549435e-38", "%f", &f); CHECK_EQ_I(n, 1); CHECK(tc_fbits(f) == 0x00800000u); }
TC_CASE(flt_max) { float f = 0; int n = sscanf("3.4028235e38", "%f", &f); CHECK_EQ_I(n, 1); CHECK(tc_fbits(f) == 0x7f7fffffu); }

int main(void) { tc_begin(); TC_RUN(min_denorm); TC_RUN(dbl_min); TC_RUN(dbl_max); TC_RUN(flt_min); TC_RUN(flt_max); return tc_end(); }
