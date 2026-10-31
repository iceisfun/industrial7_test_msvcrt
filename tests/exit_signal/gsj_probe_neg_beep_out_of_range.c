/* TC
 * id:    gsj_probe_neg_beep_out_of_range
 * area:  exit_signal
 * funcs:
 * procs: _beep
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Beep (frequency 37..32767)
 * note:  Probes that _beep with invalid frequencies (36 and 40000; (0,0) is deliberately avoided: the beep driver treats it as a stop request) returns without crashing, and (own case) that the
 *        last error becomes ERROR_INVALID_PARAMETER. An in-range frequency is never used.
 */
#include "tc.h"

typedef void (__cdecl *beep_t)(unsigned, unsigned);
static beep_t p_beep;

TC_CASE(below_min) { if (!p_beep) TC_SKIP("no _beep"); p_beep(36, 0); CHECK(1); }
TC_CASE(too_high) { if (!p_beep) TC_SKIP("no _beep"); p_beep(40000, 0); CHECK(1); }
TC_CASE(last_error)
{
    if (!p_beep) TC_SKIP("no _beep");
    SetLastError(0); p_beep(40000, 0);
    CHECK_EQ_U(GetLastError(), 87);
}
int main(void)
{
    tc_begin(); p_beep = (beep_t)tc_crt_proc("_beep");
    TC_RUN(below_min); TC_RUN(too_high); TC_RUN(last_error);
    return tc_end();
}
