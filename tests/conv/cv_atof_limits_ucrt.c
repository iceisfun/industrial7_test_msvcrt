/* TC
 * id:    cv_atof_limits_ucrt
 * area:  conv
 * funcs: atof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3, IEC 60559 correct rounding (UCRT strtod is correctly rounded); DBL_MAX/DBL_MIN/denorm_min decimals
 */
#include "tc.h"
#include <stdlib.h>
#include <float.h>

static double F(const char *volatile s) { return atof(s); }

TC_CASE(dbl_max) { CHECK_BITS_D(F("1.7976931348623157e308"), DBL_MAX); }
TC_CASE(dbl_min) { CHECK_BITS_D(F("2.2250738585072014e-308"), DBL_MIN); }
TC_CASE(denorm_min) { CHECK_BITS_D(F("4.9406564584124654e-324"), 4.9406564584124654e-324); CHECK(F("4.9406564584124654e-324") > 0.0); }
TC_CASE(neg_underflow_is_neg_zero) { CHECK_BITS_D(F("-1e-400"), -0.0); }

int main(void) { tc_begin(); TC_RUN(dbl_max); TC_RUN(dbl_min); TC_RUN(denorm_min); TC_RUN(neg_underflow_is_neg_zero); return tc_end(); }
