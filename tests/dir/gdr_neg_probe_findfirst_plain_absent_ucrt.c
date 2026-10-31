/* TC
 * id:    gdr_neg_probe_findfirst_plain_absent_ucrt
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _findfirst _findnext _wfindfirst _wfindnext _stat _fstat _wstat
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   Universal CRT headers: _findfirst/_stat etc. are inline wrappers over the explicit-width exports
 * note:  PROBE: real ucrtbase.dll is believed to export only the explicit-width variants (_findfirst32, _findfirst64i32,
 *        _stat32, _stat64i32, ...), not the un-suffixed time_t-dependent names. Any non-NULL result is a finding.
 */
#include "tc.h"

TC_CASE(absent_find) { CHECK(tc_crt_proc("_findfirst") == NULL); CHECK(tc_crt_proc("_findnext") == NULL); }
TC_CASE(absent_wfind) { CHECK(tc_crt_proc("_wfindfirst") == NULL); CHECK(tc_crt_proc("_wfindnext") == NULL); }
TC_CASE(absent_stat) { CHECK(tc_crt_proc("_stat") == NULL); CHECK(tc_crt_proc("_fstat") == NULL); CHECK(tc_crt_proc("_wstat") == NULL); }
TC_CASE(explicit_present)
{
    /* control: the explicit-width names must exist, otherwise the probe above says nothing */
    CHECK(tc_crt_proc("_findfirst32") != NULL); CHECK(tc_crt_proc("_findfirst64i32") != NULL);
    CHECK(tc_crt_proc("_stat32") != NULL); CHECK(tc_crt_proc("_stat64i32") != NULL);
}
int main(void) { tc_begin(); TC_RUN(explicit_present); TC_RUN(absent_find); TC_RUN(absent_wfind); TC_RUN(absent_stat); return tc_end(); }
