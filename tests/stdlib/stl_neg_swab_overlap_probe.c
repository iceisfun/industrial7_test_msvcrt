/* TC
 * id:    stl_neg_swab_overlap_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swab (overlap not specified)
 * note:  Probes that in-place _swab (dest == src) does not crash and swaps pairwise ("abcd" -> "badc"),
 *        as a pairwise-temp implementation would. Overlap is unspecified by the docs.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_CASE(in_place)
{
    char b[5]; strcpy(b, "abcd");
    _swab(b, b, 4);
    CHECK_STR(b, "badc");
}
int main(void) { tc_begin(); TC_RUN(in_place); return tc_end(); }
