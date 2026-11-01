/* TC
 * id:    mf_l_variants_exports_probe
 * area:  math_float
 * funcs:
 * procs: sinl cosl tanl asinl acosl atanl sinhl coshl acoshl asinhl atanhl cbrtl erfl exp2l expm1l
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs math library "Requirements" tables
 * note:  D: the real export table of the reference ucrtbase.dll (Win10 19045) has NO sinl cosl tanl asinl acosl atanl sinhl coshl (they are header inlines onto the double versions); it does export acoshl asinhl atanhl cbrtl erfl exp2l expm1l (and many more). Name set fixed accordingly (evidence: the reference VM run itself, GetProcAddress on every name, two runs; no Microsoft binary copied into the repo). msvcr120 not verified.
 *        Was PROBE: which long-double (l-suffix) math names are real exports of the DLL (versus header inlines).
 */
#include "tc.h"

TC_CASE(sinl_absent) { CHECK(tc_crt_proc("sinl") == NULL); }
TC_CASE(cosl_absent) { CHECK(tc_crt_proc("cosl") == NULL); }
TC_CASE(tanl_absent) { CHECK(tc_crt_proc("tanl") == NULL); }
TC_CASE(asinl_absent) { CHECK(tc_crt_proc("asinl") == NULL); }
TC_CASE(acosl_absent) { CHECK(tc_crt_proc("acosl") == NULL); }
TC_CASE(atanl_absent) { CHECK(tc_crt_proc("atanl") == NULL); }
TC_CASE(sinhl_absent) { CHECK(tc_crt_proc("sinhl") == NULL); }
TC_CASE(rest_absent_and_present)
{
    CHECK(tc_crt_proc("coshl") == NULL);
    CHECK(tc_crt_proc("acoshl") != NULL);
    CHECK(tc_crt_proc("asinhl") != NULL);
    CHECK(tc_crt_proc("atanhl") != NULL);
    CHECK(tc_crt_proc("cbrtl") != NULL);
    CHECK(tc_crt_proc("erfl") != NULL);
    CHECK(tc_crt_proc("exp2l") != NULL);
    CHECK(tc_crt_proc("expm1l") != NULL);
}

int main(void)
{
    tc_begin();
    TC_RUN(sinl_absent);
    TC_RUN(cosl_absent);
    TC_RUN(tanl_absent);
    TC_RUN(asinl_absent);
    TC_RUN(acosl_absent);
    TC_RUN(atanl_absent);
    TC_RUN(sinhl_absent);
    TC_RUN(rest_absent_and_present);
    return tc_end();
}
