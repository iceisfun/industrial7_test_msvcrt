/* TC
 * id:    pfs_neg_probe_width_precision_intmax_str
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf (count exceeded -> -1, only count bytes written); C11 7.21.6.1 (negative * precision taken as omitted)
 * note:  PROBE: huge string widths (INT_MAX via *) must not overrun a small buffer: _snprintf returns -1 with sentinels beyond count intact. Also %.*s with INT_MIN precision prints the whole string and INT_MAX precision on a short string prints it whole. Width 1<<20 (INT_MAX would take minutes of padding) is used instead of INT_MAX to keep run time short.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 0x55; }
static const char *volatile f_star = "%*s";
static const char *volatile f_pstar = "%.*s";
static volatile int wbig = 1 << 20;
static volatile int imin = INT_MIN;
static volatile int imax = INT_MAX;

TC_CASE(big_width_no_overrun)
{
    char b[16]; int n; fill(b);
    n = _snprintf(b, 8, f_star, wbig, "a");
    CHECK_EQ_I(n, -1);
    CHECK(b[8] == 0x55 && b[9] == 0x55 && b[15] == 0x55);
}
TC_CASE(sprintf_s_big_width)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = sprintf_s(b, 8, f_star, wbig, "a");
    CHECK_EQ_I(n, -1); CHECK(b[0] == 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(precision_int_min)
{
    char b[16]; int n; fill(b);
    n = _snprintf(b, 16, f_pstar, imin, "abc");
    CHECK_EQ_I(n, 3); CHECK_MEM(b, "abc", 3);
}
TC_CASE(precision_int_max)
{
    char b[16]; int n; fill(b);
    n = _snprintf(b, 16, f_pstar, imax, "abc");
    CHECK_EQ_I(n, 3); CHECK_MEM(b, "abc", 3);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(big_width_no_overrun); TC_RUN(sprintf_s_big_width); TC_RUN(precision_int_min); TC_RUN(precision_int_max);
    return tc_end();
}
