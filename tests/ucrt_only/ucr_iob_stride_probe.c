/* TC
 * id:    ucr_iob_stride_probe
 * area:  ucrt_only
 * funcs: __acrt_iob_func
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   UCRT implementation detail
 * note:  PROBES whether the three std FILE objects are laid out at a constant stride (an array), i.e.
 *        p(1)-p(0) == p(2)-p(1). Not promised by docs.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned v0 = 0, v1 = 1, v2 = 2;

TC_CASE(stride)
{
    char *a = (char *)__acrt_iob_func(v0), *b = (char *)__acrt_iob_func(v1), *c = (char *)__acrt_iob_func(v2);
    CHECK((b - a) == (c - b));
}
int main(void) { tc_begin(); TC_RUN(stride); return tc_end(); }
