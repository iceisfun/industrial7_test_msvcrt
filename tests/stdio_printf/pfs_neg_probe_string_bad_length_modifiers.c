/* TC
 * id:    pfs_neg_probe_string_bad_length_modifiers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   UCRT printf: unsupported length-modifier / conversion combinations are invalid format strings -> invalid parameter handler, errno EINVAL, -1
 * note:  PROBE: %hhs, %lls and %Lc are rejected (-1, handler 1, EINVAL). %zs and %js are probed only for self-consistency: either rejected the same way, or accepted and printed as plain %s. The final case checks that %hs still works afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f_hhs = "%hhs";
static const char *volatile f_lls = "%lls";
static const char *volatile f_Lc = "%Lc";
static const char *volatile f_zs = "%zs";
static const char *volatile f_js = "%js";
static const char *volatile f_hs = "%hs";
static volatile int xc = 'x';

TC_CASE(hhs)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_hhs, "x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(lls)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_lls, "x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(Lc)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_Lc, xc);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(zs_consistent)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_zs, "x");
    if (n == -1) { CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
    else { CHECK_EQ_I(n, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0); }
}
TC_CASE(js_consistent)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_js, "x");
    if (n == -1) { CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
    else { CHECK_EQ_I(n, 1); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0); }
}
TC_CASE(control_hs_after)
{
    char b[16]; int n; tc_iph_count = 0;
    n = sprintf(b, f_hs, "ok");
    CHECK_EQ_I(n, 2); CHECK_STR(b, "ok"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(hhs); TC_RUN(lls); TC_RUN(Lc); TC_RUN(zs_consistent); TC_RUN(js_consistent); TC_RUN(control_hs_after);
    return tc_end();
}
