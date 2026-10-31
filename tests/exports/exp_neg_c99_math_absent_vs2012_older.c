/* TC
 * id:    exp_neg_c99_math_absent_vs2012_older
 * area:  exports
 * funcs:
 * procs: round trunc cbrt log2 expm1 log1p exp2 tgamma lgamma erf erfc fmax fmin fma remainder remquo nearbyint rint lround llround acosh asinh atanh scalbn ilogb
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS What's new in Visual C++ 2013 (C99 math library added to msvcr120)
 * note:  Probing: these C99 math names are not exported by msvcr80/90/100/110. The docs only say C99 math arrived with VS2013; export-table absence is not itself documented, and msvcr110 is the least certain.
 */
#include "tc.h"

static void chk(const char *const *n, int cnt)
{
    int i; for (i = 0; i < cnt; i++) CHECK(tc_crt_proc(n[i]) == NULL);
}
TC_CASE(rounding_absent) { static const char *const n[] = {"round", "trunc", "nearbyint", "rint", "lround", "llround"}; chk(n, 6); }
TC_CASE(exp_log_absent) { static const char *const n[] = {"cbrt", "log2", "expm1", "log1p", "exp2"}; chk(n, 5); }
TC_CASE(gamma_erf_absent) { static const char *const n[] = {"tgamma", "lgamma", "erf", "erfc"}; chk(n, 4); }
TC_CASE(minmax_fma_absent) { static const char *const n[] = {"fmax", "fmin", "fma", "remainder", "remquo"}; chk(n, 5); }
TC_CASE(hyperbolic_inverse_absent) { static const char *const n[] = {"acosh", "asinh", "atanh"}; chk(n, 3); }
TC_CASE(scalbn_ilogb_absent) { static const char *const n[] = {"scalbn", "ilogb"}; chk(n, 2); }

int main(void)
{
    tc_begin();
    TC_RUN(rounding_absent);
    TC_RUN(exp_log_absent);
    TC_RUN(gamma_erf_absent);
    TC_RUN(minmax_fma_absent);
    TC_RUN(hyperbolic_inverse_absent);
    TC_RUN(scalbn_ilogb_absent);
    return tc_end();
}
