/* TC
 * id:    nfx_abort_behavior_flags_roundtrip
 * area:  negative_fatal
 * kind:  positive
 * funcs: 
 * procs: _set_abort_behavior
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_abort_behavior (_WRITE_ABORT_MSG=1, _CALL_REPORTFAULT=2)
 * note:  In-process; restores the original flags.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
TC_CASE(roundtrip)
{
    sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
    unsigned int orig;
    CHECK(sab);
    if (!sab) return;
    orig = sab(0, 3);                 /* clear both, returns previous */
    CHECK_EQ_U(sab(1, 1), 0);         /* previous 0, now 1 */
    CHECK_EQ_U(sab(0, 1), 1);         /* previous 1, now 0 */
    CHECK_EQ_U(sab(0, 1), 0);
    sab(0, 3);
    CHECK_EQ_U(sab(0, 0), 0);         /* nothing selected by mask: state still 0 */
    CHECK_EQ_U(sab(2, 2), 0);
    CHECK_EQ_U(sab(0, 0), 2);
    CHECK_EQ_U(sab(0, 2), 2);
    CHECK_EQ_U(sab(0, 0), 0);
    sab(orig, 3);
}
int main(void)
{
    tc_begin();
    TC_RUN(roundtrip);
    return tc_end();
}
