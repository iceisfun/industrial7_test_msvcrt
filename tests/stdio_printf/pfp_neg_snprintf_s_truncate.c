/* TC
 * id:    pfp_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _snprintf_s: _TRUNCATE truncates, returns -1, no handler
 * note:  conf observed because the %p output (16 zero-padded uppercase hex digits on x64) is not spelled out in the docs
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(truncate_flag)
{
    char b[16]; volatile void *p = (void *)(size_t)0xABCDEF; int r, i;
    if (!TC_X64) TC_SKIP("x64 only");
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, _TRUNCATE, "%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "0000000");
    CHECK_EQ_I(b[7], 0);
    CHECK_EQ_I(tc_iph_count, 0);
    for (i = 8; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(fits_control)
{
    char b[32]; volatile void *p = (void *)(size_t)0xABCDEF; int r;
    if (!TC_X64) TC_SKIP("x64 only");
    tc_iph_count = 0;
    r = _snprintf_s(b, 32, _TRUNCATE, "%p", p);
    CHECK_EQ_I(r, 16);
    CHECK_STR(b, "0000000000ABCDEF");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(truncate_flag); TC_RUN(fits_control); return tc_end(); }
