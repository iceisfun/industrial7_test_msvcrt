/* TC
 * id:    mf_l_variants_exports_probe
 * area:  math_float
 * funcs:
 * procs: sinl cosl tanl asinl acosl atanl sinhl coshl
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs math library "Requirements" tables
 * note:  PROBE: which long-double (l-suffix) math names are real exports of the DLL (versus header inlines).
 *        One case per name so a real-Windows run shows exactly which ones exist. Likely true for ucrtbase.
 */
#include "tc.h"

TC_CASE(sinl) { CHECK(tc_crt_proc("sinl") != NULL); }
TC_CASE(cosl) { CHECK(tc_crt_proc("cosl") != NULL); }
TC_CASE(tanl) { CHECK(tc_crt_proc("tanl") != NULL); }
TC_CASE(asinl) { CHECK(tc_crt_proc("asinl") != NULL); }
TC_CASE(acosl) { CHECK(tc_crt_proc("acosl") != NULL); }
TC_CASE(atanl) { CHECK(tc_crt_proc("atanl") != NULL); }
TC_CASE(sinhl) { CHECK(tc_crt_proc("sinhl") != NULL); }
TC_CASE(coshl) { CHECK(tc_crt_proc("coshl") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(sinl);
    TC_RUN(cosl);
    TC_RUN(tanl);
    TC_RUN(asinl);
    TC_RUN(acosl);
    TC_RUN(atanl);
    TC_RUN(sinhl);
    TC_RUN(coshl);
    return tc_end();
}
