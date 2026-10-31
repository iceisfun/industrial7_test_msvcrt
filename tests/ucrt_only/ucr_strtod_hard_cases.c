/* TC
 * id:    ucr_strtod_hard_cases
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.3 (<=DECIMAL_DIG=17 significant digits must round correctly, F.5.2); MS docs strtod
 * note:  UCRT is correctly rounded since VS2015; older CRTs were not, hence win10 only.
 *       All inputs here have <=17 significant digits (standard-mandated correct rounding); longer inputs are in ucr_strtod_long_digits_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <float.h>

static const char *volatile v;
TC_CASE(point_one) { char *e; v = "0.1"; CHECK_BITS_D(strtod((const char *)v, &e), 0x1.999999999999ap-4); CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x3FB999999999999AULL); }
TC_CASE(tie_to_even) { char *e; v = "9007199254740993"; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), tc_dbits(9007199254740992.0));
  v = "9007199254740995"; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), tc_dbits(9007199254740996.0)); }
TC_CASE(largest_subnormal) { char *e; v = "2.2250738585072011e-308"; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x000FFFFFFFFFFFFFULL); }
TC_CASE(dbl_max) { char *e; v = "1.7976931348623157e308"; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 0x7FEFFFFFFFFFFFFFULL); }
TC_CASE(min_denormal) { char *e; v = "4.9406564584124654e-324"; CHECK_EQ_U(tc_dbits(strtod((const char *)v, &e)), 1); }

int main(void) { tc_begin(); TC_RUN(point_one); TC_RUN(tie_to_even); TC_RUN(largest_subnormal); TC_RUN(dbl_max); TC_RUN(min_denormal); return tc_end(); }
