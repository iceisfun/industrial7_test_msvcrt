/* TC
 * id:    ct_neg_isblank_absent_msvcr110_probe
 * area:  ctype
 * funcs:
 * procs: isblank iswblank
 * crt:   msvcr100 msvcr110 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs isblank (VS2013+)
 * note:  PROBE: whether msvcr100.dll (VS2010), msvcr110.dll (VS2012) and msvcrt.dll lack isblank/iswblank. Believed absent (mingw import lib lacks them).
 */
#include "tc.h"

TC_CASE(isblank_null) { CHECK(tc_crt_proc("isblank") == NULL); }
TC_CASE(iswblank_null) { CHECK(tc_crt_proc("iswblank") == NULL); }

int main(void) { tc_begin(); TC_RUN(isblank_null); TC_RUN(iswblank_null); return tc_end(); }
