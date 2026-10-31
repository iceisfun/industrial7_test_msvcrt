/* TC
 * id:    stl_neg_swab_no_overrun_guard
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swab: copies n bytes from src to dest swapping adjacent byte pairs; n should be even
 * note:  Even n only. Odd n is unspecified by the docs and lives in stl_neg_swab_odd_count_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static const char src0[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };

TC_CASE(even_six)
{
    unsigned char buf[10]; char s[8]; static const unsigned char want[6] = { '2', '1', '4', '3', '6', '5' };
    memset(buf, 0xAA, sizeof buf); memcpy(s, src0, 8);
    _swab(s, (char *)buf + 2, 6);
    CHECK_MEM(buf + 2, want, 6); CHECK_EQ_U(buf[0], 0xAA); CHECK_EQ_U(buf[1], 0xAA); CHECK_EQ_U(buf[8], 0xAA); CHECK_EQ_U(buf[9], 0xAA);
    CHECK_MEM(s, src0, 8);
}
TC_CASE(two_only)
{
    unsigned char buf[6]; char s[8]; memset(buf, 0xAA, sizeof buf); memcpy(s, src0, 8);
    _swab(s, (char *)buf + 2, 2);
    CHECK_EQ_U(buf[2], '2'); CHECK_EQ_U(buf[3], '1'); CHECK_EQ_U(buf[1], 0xAA); CHECK_EQ_U(buf[4], 0xAA); CHECK_EQ_U(buf[5], 0xAA);
    CHECK_MEM(s, src0, 8);
}
int main(void) { tc_begin(); TC_RUN(even_six); TC_RUN(two_only); return tc_end(); }
