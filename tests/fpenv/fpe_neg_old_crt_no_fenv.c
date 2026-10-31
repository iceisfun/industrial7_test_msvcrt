/* TC
 * id:    fpe_neg_old_crt_no_fenv
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: fegetround fesetround feraiseexcept fetestexcept
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fenv.h / fegetround "Requirements": C99 <fenv.h> supported from Visual Studio 2013 (msvcr120)
 * note:  PROBES that pre-VS2013 runtimes (msvcr110 and older, msvcrt.dll) do not export the C99 fenv functions.
 *        Legacy FP control is via _controlfp, which every target must have.
 */
#include "tc.h"
#include <float.h>

TC_CASE(controlfp_present) { CHECK(tc_crt_proc("_controlfp") != NULL); CHECK(_controlfp(0, 0) != 0xffffffffu); }
TC_CASE(fegetround_absent) { CHECK(tc_crt_proc("fegetround") == NULL); }
TC_CASE(fesetround_absent) { CHECK(tc_crt_proc("fesetround") == NULL); }
TC_CASE(feraiseexcept_absent) { CHECK(tc_crt_proc("feraiseexcept") == NULL); }
TC_CASE(fetestexcept_absent) { CHECK(tc_crt_proc("fetestexcept") == NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(controlfp_present); TC_RUN(fegetround_absent); TC_RUN(fesetround_absent);
    TC_RUN(feraiseexcept_absent); TC_RUN(fetestexcept_absent);
    return tc_end();
}
