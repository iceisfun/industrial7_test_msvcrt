/* TC
 * id:    gcv_neg_swab_overlap_probe
 * area:  conv
 * kind:  negative
 * funcs: _swab
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swab (overlap unspecified)
 * note:  PROBE: in-place _swab(buf, buf, 4) yields "BADC" (each pair read before written); shifted overlap only must not crash.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(in_place)
{
    char b[5] = {'A','B','C','D',0};
    _swab(b, b, 4);
    CHECK_STR(b, "BADC");
}
TC_CASE(shifted_no_crash)
{
    char b[8] = {'A','B','C','D','E',0,0,0};
    _swab(b, b + 1, 4);
    CHECK_EQ_I(b[0], 'A');
}
TC_CASE(in_place_odd1)
{
    char b[3] = {'Z','Y',0};
    _swab(b, b, 1);
    CHECK_STR(b, "ZY");
}
int main(void)
{
    tc_begin();
    TC_RUN(in_place); TC_RUN(shifted_no_crash); TC_RUN(in_place_odd1);
    return tc_end();
}
