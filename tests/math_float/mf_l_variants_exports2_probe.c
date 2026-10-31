/* TC
 * id:    mf_l_variants_exports2_probe
 * area:  math_float
 * funcs:
 * procs: tanhl expl logl log10l powl sqrtl copysignl fmodl
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs math library "Requirements" tables
 * note:  PROBE: which long-double (l-suffix) math names are real exports of the DLL (versus header inlines).
 *        One case per name so a real-Windows run shows exactly which ones exist. Likely true for ucrtbase.
 */
#include "tc.h"

TC_CASE(tanhl) { CHECK(tc_crt_proc("tanhl") != NULL); }
TC_CASE(expl) { CHECK(tc_crt_proc("expl") != NULL); }
TC_CASE(logl) { CHECK(tc_crt_proc("logl") != NULL); }
TC_CASE(log10l) { CHECK(tc_crt_proc("log10l") != NULL); }
TC_CASE(powl) { CHECK(tc_crt_proc("powl") != NULL); }
TC_CASE(sqrtl) { CHECK(tc_crt_proc("sqrtl") != NULL); }
TC_CASE(copysignl) { CHECK(tc_crt_proc("copysignl") != NULL); }
TC_CASE(fmodl) { CHECK(tc_crt_proc("fmodl") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(tanhl);
    TC_RUN(expl);
    TC_RUN(logl);
    TC_RUN(log10l);
    TC_RUN(powl);
    TC_RUN(sqrtl);
    TC_RUN(copysignl);
    TC_RUN(fmodl);
    return tc_end();
}
