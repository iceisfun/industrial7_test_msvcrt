/* TC
 * id:    cv_neg_cvt_not_exported_legacy
 * area:  conv
 * funcs:
 * procs: _strtoi64 _wcstoi64 strtoll strtoull strtof wcstoll
 * crt:   msvcr90 msvcr100 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtoll/strtof (VS2013+ / UCRT); _strtoi64 (all versions)
 * note:  Probes export presence. _strtoi64/_wcstoi64 must exist in all listed CRTs. C99 names strtoll/strtoull/strtof/wcstoll
 *        are expected ABSENT from msvcrt.dll and msvcr90.dll (introduced by VS2013's msvcr120); msvcr100 status is not asserted.
 *        Note: modern Windows msvcrt.dll may differ from the Windows 7 one.
 */
#include "tc.h"

TC_CASE(i64_present) { CHECK(tc_crt_proc("_strtoi64") != NULL); CHECK(tc_crt_proc("_wcstoi64") != NULL); }
TC_CASE(c99_absent)
{
#if TC_CRT_VER <= 90
    CHECK(tc_crt_proc("strtoll") == NULL);
    CHECK(tc_crt_proc("strtoull") == NULL);
    CHECK(tc_crt_proc("strtof") == NULL);
    CHECK(tc_crt_proc("wcstoll") == NULL);
#else
    TC_SKIP("msvcr100 presence not asserted");
#endif
}
int main(void) { tc_begin(); TC_RUN(i64_present); TC_RUN(c99_absent); return tc_end(); }
