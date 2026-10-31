/* TC
 * id:    stl_neg_swab_odd_count_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swab ("n should be even"; odd n not specified)
 * note:  PROBES odd n=5: expectation is that the first 4 bytes are swapped pairwise, nothing is written beyond the 5-byte window
 *        (dest[5..]) and the guard bytes before dest are untouched. Whether dest[4] is written is NOT asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static const char src0[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };

TC_CASE(odd_five)
{
    unsigned char buf[10]; char s[8]; static const unsigned char want[4] = { '2', '1', '4', '3' };
    memset(buf, 0xAA, sizeof buf); memcpy(s, src0, 8);
    _swab(s, (char *)buf + 2, 5);
    CHECK_MEM(buf + 2, want, 4); CHECK_EQ_U(buf[0], 0xAA); CHECK_EQ_U(buf[1], 0xAA);
    CHECK_EQ_U(buf[7], 0xAA); CHECK_EQ_U(buf[8], 0xAA); CHECK_EQ_U(buf[9], 0xAA);
    CHECK_MEM(s, src0, 8);
}
int main(void) { tc_begin(); TC_RUN(odd_five); return tc_end(); }
