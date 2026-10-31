/* TC
 * id:    tm_neg_probe_exports_absent_old
 * area:  time
 * kind:  negative
 * funcs: 
 * procs: _time32 _mktime32
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   mingw-w64 import library for msvcrt.dll
 * note:  PROBES that msvcrt.dll (6.0 / system) does not export _time32 and _mktime32 (the 32-bit explicit names appeared
 *        with the VS2005 CRT).
 */
#include "tc.h"

TC_CASE(time32_absent)   { CHECK(tc_crt_proc("_time32") == NULL); }
TC_CASE(mktime32_absent) { CHECK(tc_crt_proc("_mktime32") == NULL); }
TC_CASE(time_present_sanity) { CHECK(tc_crt_proc("time") != NULL); }

int main(void)
{
    tc_begin();
    TC_RUN(time32_absent);
    TC_RUN(mktime32_absent);
    TC_RUN(time_present_sanity);
    return tc_end();
}
