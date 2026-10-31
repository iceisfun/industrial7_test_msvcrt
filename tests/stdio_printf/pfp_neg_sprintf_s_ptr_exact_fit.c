/* TC
 * id:    pfp_neg_sprintf_s_ptr_exact_fit
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s: buffer must hold output plus NUL; overflow -> handler, buf[0]=0
 * note:  conf observed: relies on %p being exactly 16 hex digits on x64
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(size16)
{
    char b[24]; volatile void *p = (void *)(size_t)0xABC; int r, i;
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 24; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = sprintf_s(b, 16, "%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    for (i = 16; i < 24; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(size17)
{
    char b[24]; volatile void *p = (void *)(size_t)0xABC; int r;
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_count = 0;
    r = sprintf_s(b, 17, "%p", p);
    CHECK_EQ_I(r, 16);
    CHECK_STR(b, "0000000000000ABC");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(size16); TC_RUN(size17); return tc_end(); }
