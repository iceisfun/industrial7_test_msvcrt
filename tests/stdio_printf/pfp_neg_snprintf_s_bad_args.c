/* TC
 * id:    pfp_neg_snprintf_s_bad_args
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, Parameter validation: NULL buffer / zero size / NULL format -> handler, -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(null_buf)
{
    char *volatile nb = NULL; int r;
    tc_iph_count = 0;
    r = _snprintf_s(nb, 8, 4, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size0)
{
    char b[4] = {'Z', 'Z', 'Z', 'Z'}; volatile size_t z = 0; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, z, 4, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'Z');
}
TC_CASE(null_fmt)
{
    char b[8] = {'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'}; const char *volatile f = NULL; int r;
    tc_iph_count = 0;
    r = _snprintf_s(b, 8, 4, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(size0); TC_RUN(null_fmt); return tc_end(); }
