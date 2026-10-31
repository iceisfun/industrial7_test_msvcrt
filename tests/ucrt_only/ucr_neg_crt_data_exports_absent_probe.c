/* TC
 * id:    ucr_neg_crt_data_exports_absent_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __p__fmode
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   UCRT removed CRT data exports; accessors __p__fmode/__p__commode/__p__environ/__p__pgmptr are used instead
 * note:  PROBES that ucrtbase.dll has no exported data symbols _fmode, _commode, _environ, _pgmptr, and that the __p__* accessors exist.
 */
#include "tc.h"

TC_CASE(no_data) { CHECK(tc_crt_proc("_fmode") == NULL); CHECK(tc_crt_proc("_commode") == NULL); CHECK(tc_crt_proc("_environ") == NULL); CHECK(tc_crt_proc("_pgmptr") == NULL); }
TC_CASE(accessors) { CHECK(tc_crt_proc("__p__fmode") != NULL); CHECK(tc_crt_proc("__p__commode") != NULL); CHECK(tc_crt_proc("__p__environ") != NULL); CHECK(tc_crt_proc("__p__pgmptr") != NULL); }

int main(void) { tc_begin(); TC_RUN(no_data); TC_RUN(accessors); return tc_end(); }
