/* TC
 * id:    gcx_neg_probe_missing_win7_exports
 * area:  math_c99
 * funcs:
 * procs: cabs carg cimag creal conj norm
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs cabs/carg/creal ("Visual Studio 2013 and later" C99 complex support)
 * note:  PROBE: C99 complex.h functions were first added in VS2013 (msvcr120); older CRTs are assumed to export only
 *        the legacy _cabs, so GetProcAddress of the C99 names must return NULL.
 */
#include "tc.h"

TC_CASE(cabs_absent)  { CHECK(tc_crt_proc("cabs") == NULL); CHECK(tc_crt_proc("cabsf") == NULL); }
TC_CASE(carg_absent)  { CHECK(tc_crt_proc("carg") == NULL); CHECK(tc_crt_proc("cargl") == NULL); }
TC_CASE(creal_absent) { CHECK(tc_crt_proc("creal") == NULL); }
TC_CASE(cimag_absent) { CHECK(tc_crt_proc("cimag") == NULL); }
TC_CASE(conj_absent)  { CHECK(tc_crt_proc("conj") == NULL); }
TC_CASE(norm_absent)  { CHECK(tc_crt_proc("norm") == NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(cabs_absent); TC_RUN(carg_absent); TC_RUN(creal_absent);
    TC_RUN(cimag_absent); TC_RUN(conj_absent); TC_RUN(norm_absent);
    return tc_end();
}
