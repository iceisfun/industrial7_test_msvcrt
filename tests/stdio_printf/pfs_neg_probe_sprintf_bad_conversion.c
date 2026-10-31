/* TC
 * id:    pfs_neg_probe_sprintf_bad_conversion
 * area:  stdio_printf
 * funcs: sprintf

 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   ISO C 7.21.6.1p9 leaves invalid conversion specs undefined
 * note:  PROBE: ucrt validates the format string and treats an unknown conversion / dangling percent as an invalid parameter (-1, EINVAL). Old CRTs silently print, so only ucrt is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_y = "%y";
static const char *volatile f_pct = "abc%";
static const char *volatile f_ll = "%ll";
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
TC_CASE(unknown_y)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_y);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dangling_percent)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_pct);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(length_only)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_ll);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(unknown_y); TC_RUN(dangling_percent); TC_RUN(length_only);
    return tc_end();
}
