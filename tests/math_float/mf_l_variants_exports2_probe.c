/* TC
 * id:    mf_l_variants_exports2_probe
 * area:  math_float
 * funcs:
 * procs: exp2l expm1l log2l log1pl cbrtl fmaxl fminl copysignl remainderl
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  observed
 * ref:   MS docs math library "Requirements" tables
 * note:  OBSERVED on Win10 22H2 ucrtbase: tanhl expl logl log10l powl sqrtl fmodl (and sinl cosl tanl ceill floorl fabsl hypotl...) are NOT exported (header inlines); exp2l expm1l log2l log1pl cbrtl fmaxl fminl remainderl copysignl are. Name set replaced accordingly.
 *        PROBE: which long-double (l-suffix) math names are real exports of the DLL (versus header inlines).
 *        One case per name so a real-Windows run shows exactly which ones exist. Likely true for ucrtbase.
 */
#include "tc.h"

TC_CASE(exp2l) { CHECK(tc_crt_proc("exp2l") != NULL); }
TC_CASE(expm1l) { CHECK(tc_crt_proc("expm1l") != NULL); }
TC_CASE(log2l) { CHECK(tc_crt_proc("log2l") != NULL); }
TC_CASE(log1pl) { CHECK(tc_crt_proc("log1pl") != NULL); }
TC_CASE(cbrtl) { CHECK(tc_crt_proc("cbrtl") != NULL); }
TC_CASE(fmaxl) { CHECK(tc_crt_proc("fmaxl") != NULL); CHECK(tc_crt_proc("fminl") != NULL); }
TC_CASE(copysignl) { CHECK(tc_crt_proc("copysignl") != NULL); }
TC_CASE(remainderl) { CHECK(tc_crt_proc("remainderl") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(exp2l);
    TC_RUN(expm1l);
    TC_RUN(log2l);
    TC_RUN(log1pl);
    TC_RUN(cbrtl);
    TC_RUN(fmaxl);
    TC_RUN(copysignl);
    TC_RUN(remainderl);
    return tc_end();
}
