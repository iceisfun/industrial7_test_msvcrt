/* TC
 * id:    pff_e_denormal_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 e
 * note:  PROBE: do pre-UCRT CRTs print denormals and DBL_MIN correctly to 15 fractional digits? Only 15-digit results are asserted, which both exact and 17-digit-based algorithms must reproduce.
 */
#include "tc.h"
#include <stdio.h>
#include <float.h>

static volatile double v;
TC_CASE(denorm_min) { char b[64]; v = 4.9406564584124654e-324; sprintf(b, "%.15e", v); CHECK_STR(b, "4.940656458412465e-324"); }
TC_CASE(denorm_min_default) { char b[64]; v = 4.9406564584124654e-324; sprintf(b, "%e", v); CHECK_STR(b, "4.940656e-324"); }
TC_CASE(dbl_min)    { char b[64]; v = DBL_MIN; sprintf(b, "%.15e", v); CHECK_STR(b, "2.225073858507201e-308"); }

int main(void) { tc_begin(); TC_RUN(denorm_min); TC_RUN(denorm_min_default); TC_RUN(dbl_min); return tc_end(); }
