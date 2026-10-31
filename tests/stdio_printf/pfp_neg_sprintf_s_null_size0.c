/* TC
 * id:    pfp_neg_sprintf_s_null_size0
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, Parameter validation: NULL buffer/format, size 0 -> handler, -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(null_buf)
{
    char *volatile nb = NULL; volatile void *p = (void *)(size_t)0xABC; int r;
    tc_iph_count = 0;
    r = sprintf_s(nb, 16, "%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size0)
{
    char b[4] = {'Z', 'Z', 'Z', 'Z'}; volatile size_t z = 0; volatile void *p = (void *)(size_t)0xABC; int r;
    tc_iph_count = 0;
    r = sprintf_s(b, z, "%p", p);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[1], 'Z');
}
TC_CASE(null_fmt)
{
    char b[16]; const char *volatile f = NULL; int r;
    b[0] = 'Z';
    tc_iph_count = 0;
    r = sprintf_s(b, 16, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(size0); TC_RUN(null_fmt); return tc_end(); }
