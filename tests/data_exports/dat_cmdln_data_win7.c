/* TC
 * id:    dat_cmdln_data_win7
 * area:  data_exports
 * kind:  positive
 * funcs:
 * procs: __p__acmdln _acmdln _wcmdln
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   Win7-era CRTs export _acmdln/_wcmdln as data
 * note:  PROBE: __p__acmdln may be absent from the x64 msvcr80/msvcrt DLLs (mingw only synthesises it); that case is skipped. Data exports resolved with GetProcAddress. ucrtbase does not export them.
 */
#include "tc.h"

TC_DECL_PROC(char **, __p__acmdln, (void));

TC_CASE(acmdln_nonempty) { char **p = (char **)tc_crt_proc("_acmdln"); CHECK(p != NULL); if (p) { CHECK(*p != NULL); if (*p) CHECK((*p)[0] != 0); } }
TC_CASE(acmdln_equals_accessor) { char **p = (char **)tc_crt_proc("_acmdln"); CHECK(p != NULL); if (p) { if (!tc_p___p__acmdln) TC_SKIP("accessor not exported"); CHECK_EQ_P(*p, *tc_p___p__acmdln()); } }
int main(void) { tc_begin(); TC_LOAD_PROC(__p__acmdln); TC_RUN(acmdln_nonempty); TC_RUN(acmdln_equals_accessor); return tc_end(); }
