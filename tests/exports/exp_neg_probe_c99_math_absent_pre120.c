/* TC
 * id:    exp_neg_probe_c99_math_absent_pre120
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _hypot
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "C99 math functions" (added in VS2013 / msvcr120)
 * note:  Probing: the VS2013 C99 additions (round, cbrt, log2, expm1, fmax, tgamma, lgamma, erf, ...) are NOT exported before msvcr120.
 *        _hypot is the control (exported by all of them).
 */
#include "tc.h"

TC_CASE(control_hypot) { CHECK(tc_crt_proc("_hypot") != NULL); }
TC_CASE(rounding_absent) { CHECK(tc_crt_proc("round") == NULL); CHECK(tc_crt_proc("trunc") == NULL); CHECK(tc_crt_proc("lround") == NULL); }
TC_CASE(roots_logs_absent) { CHECK(tc_crt_proc("cbrt") == NULL); CHECK(tc_crt_proc("log2") == NULL); CHECK(tc_crt_proc("expm1") == NULL); CHECK(tc_crt_proc("log1p") == NULL); }
TC_CASE(minmax_special_absent) { CHECK(tc_crt_proc("fmax") == NULL); CHECK(tc_crt_proc("fmin") == NULL); CHECK(tc_crt_proc("tgamma") == NULL); CHECK(tc_crt_proc("erf") == NULL); }
int main(void) { tc_begin(); TC_RUN(control_hypot); TC_RUN(rounding_absent); TC_RUN(roots_logs_absent); TC_RUN(minmax_special_absent); return tc_end(); }
