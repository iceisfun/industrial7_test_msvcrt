/* TC
 * id:    exp_neg_probe_msvcrt_no_secure_rest
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: gets_s
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Security-enhanced versions of CRT functions
 * note:  Probing: msvcrt.dll (Win7) does NOT export gets_s. Weak guess (an import library for msvcrt lacks it). The earlier claim that
 *        fopen_s/_wfopen_s/getenv_s/_putenv_s are absent was dropped: evidence points the other way, see exp_probe_msvcrt_secure_subset.
 */
#include "tc.h"

TC_CASE(gets_s_absent) { CHECK(tc_crt_proc("gets_s") == NULL); }
int main(void) { tc_begin(); TC_RUN(gets_s_absent); return tc_end(); }
