/* TC
 * id:    pfp_neg_vsprintf_s_null_args
 * area:  stdio_printf
 * funcs: vsprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vsprintf_s, Parameter validation: NULL buffer/format, size 0, invalid format -> handler, -1
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

static int vs(char *b, size_t n, const char *f, ...)
{ int r; va_list ap; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(null_buf)
{
    char *volatile nb = NULL; int r;
    tc_iph_count = 0;
    r = vs(nb, 16, "%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_fmt)
{
    char b[16]; const char *volatile f = NULL; int r;
    tc_iph_count = 0;
    r = vs(b, 16, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size0)
{
    char b[4] = {'Z', 'Z', 'Z', 'Z'}; volatile size_t z = 0; int r;
    tc_iph_count = 0;
    r = vs(b, z, "%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 'Z');
}
TC_CASE(bad_conv)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = vs(b, 16, "%y", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(size0); TC_RUN(bad_conv); return tc_end(); }
