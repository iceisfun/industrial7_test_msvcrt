/* TC
 * id:    ucr_commode_default_probe
 * area:  ucrt_only
 * funcs:
 * procs: __p__commode
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _commode (default 0 = no commit-to-disk on fflush)
 * note:  PROBE: *__p__commode() is 0 at startup (assumes mingw startup does not change it).
 */
#include "tc.h"
typedef int *(__cdecl *pc_t)(void);
TC_CASE(def) { pc_t f = (pc_t)tc_crt_proc("__p__commode"); if (!f) TC_SKIP("not exported"); CHECK(f() != NULL); if (f()) CHECK_EQ_I(*f(), 0); }
int main(void) { tc_begin(); TC_RUN(def); return tc_end(); }
