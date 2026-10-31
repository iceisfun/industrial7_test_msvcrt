/* TC
 * id:    dat_neg_msvcrt_getters_absent_probe
 * area:  data_exports
 * kind:  negative
 * funcs:
 * procs: _daylight _timezone _fmode _environ __argc __argv _pctype
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs: _get_fmode/_get_daylight/_get_timezone/_get_dstbias/_get_tzname are VS2005+ (msvcr80+)
 * note:  PROBE: Win7 msvcrt.dll (VS6-era ABI) does not export the VS2005 getter/setter functions. Each group is separate. __p___argc/__p___argv are NOT asserted absent (msvcrt.dll does export them); they are controls.
 */
#include "tc.h"

TC_CASE(fmode_getset) { CHECK(tc_crt_proc("_get_fmode") == NULL); CHECK(tc_crt_proc("_set_fmode") == NULL); }
TC_CASE(time_getters) { CHECK(tc_crt_proc("_get_daylight") == NULL); CHECK(tc_crt_proc("_get_timezone") == NULL); CHECK(tc_crt_proc("_get_dstbias") == NULL); CHECK(tc_crt_proc("_get_tzname") == NULL); }
TC_CASE(pgmptr_getters) { CHECK(tc_crt_proc("_get_pgmptr") == NULL); CHECK(tc_crt_proc("_get_wpgmptr") == NULL); }
TC_CASE(control_data)
{
    CHECK(tc_crt_proc("__argc") != NULL); CHECK(tc_crt_proc("__argv") != NULL); CHECK(tc_crt_proc("_environ") != NULL);
    CHECK(tc_crt_proc("_fmode") != NULL); CHECK(tc_crt_proc("_daylight") != NULL); CHECK(tc_crt_proc("_timezone") != NULL);
    CHECK(tc_crt_proc("_pctype") != NULL);
}
TC_CASE(control_argc_accessors) { CHECK(tc_crt_proc("__p___argc") != NULL); CHECK(tc_crt_proc("__p___argv") != NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(fmode_getset); TC_RUN(time_getters); TC_RUN(pgmptr_getters); TC_RUN(control_data); TC_RUN(control_argc_accessors);
    return tc_end();
}
