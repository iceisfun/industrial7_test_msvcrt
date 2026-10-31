/* TC
 * id:    stl_set_abort_behavior_basic
 * area:  stdlib
 * funcs: _set_abort_behavior
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_abort_behavior: new = (old & ~mask) | (flags & mask); returns the previous flags
 * note:  Only bits 1 (_WRITE_ABORT_MSG) and 2 (_CALL_REPORTFAULT) are inspected; the default value is not asserted.
 *        The test restores the original bits (tc_begin already cleared both).
 */
#include "tc.h"
#include <stdlib.h>

#define WM 1u
#define RF 2u
static volatile unsigned int v0 = 0;

TC_CASE(flags_roundtrip)
{
    unsigned int orig = _set_abort_behavior(0, 0) & (WM | RF);
    CHECK_EQ_U(_set_abort_behavior(WM | RF, WM | RF) & (WM | RF), orig);   /* returns previous */
    CHECK_EQ_U(_set_abort_behavior(v0, 0) & (WM | RF), WM | RF);           /* mask 0: query only */
    CHECK_EQ_U(_set_abort_behavior(v0, 0) & (WM | RF), WM | RF);           /* and it did not change */
    _set_abort_behavior(orig, WM | RF);
    CHECK_EQ_U(_set_abort_behavior(v0, 0) & (WM | RF), orig);
}
TC_CASE(mask_limits_changes)
{
    unsigned int orig = _set_abort_behavior(0, 0) & (WM | RF);
    _set_abort_behavior(WM | RF, WM | RF);
    _set_abort_behavior(0, WM);                       /* clear only WM */
    CHECK_EQ_U(_set_abort_behavior(0, 0) & (WM | RF), RF);
    _set_abort_behavior(WM | RF, WM);                 /* flags outside mask ignored: only WM set */
    CHECK_EQ_U(_set_abort_behavior(0, 0) & (WM | RF), WM | RF);
    _set_abort_behavior(0, RF);                       /* clear only RF */
    CHECK_EQ_U(_set_abort_behavior(0, 0) & (WM | RF), WM);
    _set_abort_behavior(orig, WM | RF);
    CHECK_EQ_U(_set_abort_behavior(0, 0) & (WM | RF), orig);
}
int main(void) { tc_begin(); TC_RUN(flags_roundtrip); TC_RUN(mask_limits_changes); return tc_end(); }
