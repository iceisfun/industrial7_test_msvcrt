/* TC
 * id:    dat_neg_ucrt_data_not_exported_more_probe
 * area:  data_exports
 * kind:  negative
 * funcs:
 * procs: __p__environ __p__fmode __p__commode __p__acmdln __p__pgmptr __timezone __daylight __dstbias __tzname
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs: UCRT replaces CRT data exports with __p__* accessor functions
 * note:  PROBE: ucrtbase.dll exports no plain data symbols (_environ, _fmode, _pctype, _timezone, ...) - only accessors (__p__* for environ/fmode/commode/acmdln/pgmptr; __timezone/__daylight/__dstbias/__tzname functions for time data).
 */
#include "tc.h"

TC_CASE(env_wenv) { CHECK(tc_crt_proc("_environ") == NULL); CHECK(tc_crt_proc("_wenviron") == NULL); }
TC_CASE(modes) { CHECK(tc_crt_proc("_fmode") == NULL); CHECK(tc_crt_proc("_commode") == NULL); }
TC_CASE(ctype) { CHECK(tc_crt_proc("_pctype") == NULL); CHECK(tc_crt_proc("_pwctype") == NULL); CHECK(tc_crt_proc("__mb_cur_max") == NULL); }
TC_CASE(cmdline) { CHECK(tc_crt_proc("_pgmptr") == NULL); CHECK(tc_crt_proc("_wpgmptr") == NULL); CHECK(tc_crt_proc("_acmdln") == NULL); CHECK(tc_crt_proc("_wcmdln") == NULL); }
TC_CASE(time) { CHECK(tc_crt_proc("_timezone") == NULL); CHECK(tc_crt_proc("_daylight") == NULL); CHECK(tc_crt_proc("_dstbias") == NULL); CHECK(tc_crt_proc("_tzname") == NULL); }
TC_CASE(controls)
{
    CHECK(tc_crt_proc("__p__environ") != NULL); CHECK(tc_crt_proc("__p__fmode") != NULL);
    CHECK(tc_crt_proc("__p__commode") != NULL); CHECK(tc_crt_proc("__p__acmdln") != NULL);
    CHECK(tc_crt_proc("__p__pgmptr") != NULL);
}
TC_CASE(controls_time)
{
    CHECK(tc_crt_proc("__timezone") != NULL); CHECK(tc_crt_proc("__daylight") != NULL);
    CHECK(tc_crt_proc("__dstbias") != NULL); CHECK(tc_crt_proc("__tzname") != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(env_wenv); TC_RUN(modes); TC_RUN(ctype); TC_RUN(cmdline); TC_RUN(time); TC_RUN(controls); TC_RUN(controls_time);
    return tc_end();
}
