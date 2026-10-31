/* TC
 * id:    pfp_neg_snprintf_s_count_edges_probe
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s (count vs sizeOfBuffer)
 * note:  PROBE: (1) count == sizeOfBuffer and the output is longer than count: expected invalid-parameter handler, -1,
 *        buffer[0]==0 (needs count+1 bytes). (2) count > sizeOfBuffer with output that fits: expected plain success.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

TC_CASE(count_eq_size_overflow)
{
    char b[16]; volatile void *p = (void *)(size_t)0xABCDEF; int r, i;
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 8, "%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    for (i = 8; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(count_gt_size_ok)
{
    char b[8]; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 100, "%d", 123);
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "123");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(count_eq_size_overflow); TC_RUN(count_gt_size_ok); return tc_end(); }
