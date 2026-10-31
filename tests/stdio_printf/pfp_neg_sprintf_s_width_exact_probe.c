/* TC
 * id:    pfp_neg_sprintf_s_width_exact_probe
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s
 * note:  PROBE: "%20p" of 0xABC on x64 yields 4 spaces + 16 zero-padded hex digits (20 chars); buffer size 20 overflows
 *        (needs 21) -> handler, -1, buf[0]==0; size 21 succeeds.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

TC_CASE(width20)
{
    char b[32]; volatile void *p = (void *)(size_t)0xABC; int r;
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_count = 0;
    r = sprintf_s(b, 20, "%20p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    tc_iph_count = 0;
    r = sprintf_s(b, 21, "%20p", p);
    CHECK_EQ_I(r, 20);
    CHECK_STR(b, "    0000000000000ABC");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(width20); return tc_end(); }
