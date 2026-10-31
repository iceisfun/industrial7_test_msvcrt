/* TC
 * id:    pff_e_extremes_precise_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (exact decimal conversion of the binary value); MS docs printf
 * note:  Exact decimal expansions: DBL_MAX = 1.7976931348623157081...e308, DBL_MIN = 2.2250738585072013830...e-308, denorm_min = 4.9406564584124654417656...e-324.
 */
#include "tc.h"
#include <stdio.h>
#include <float.h>

static volatile double v;
TC_CASE(dbl_max)  { char b[64]; v = DBL_MAX; sprintf(b, "%.17e", v); CHECK_STR(b, "1.79769313486231571e+308"); }
TC_CASE(dbl_min)  { char b[64]; v = DBL_MIN; sprintf(b, "%.17e", v); CHECK_STR(b, "2.22507385850720138e-308"); }
TC_CASE(denorm_min) { char b[64]; v = 4.9406564584124654e-324; sprintf(b, "%.20e", v); CHECK_STR(b, "4.94065645841246544177e-324"); }
TC_CASE(largest_denormal) { char b[64]; v = 2.2250738585072009e-308; sprintf(b, "%.5e", v); CHECK_STR(b, "2.22507e-308"); }

int main(void) { tc_begin(); TC_RUN(dbl_max); TC_RUN(dbl_min); TC_RUN(denorm_min); TC_RUN(largest_denormal); return tc_end(); }
