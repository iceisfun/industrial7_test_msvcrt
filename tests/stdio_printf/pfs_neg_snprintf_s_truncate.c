/* TC
 * id:    pfs_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs: _snprintf_s

 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: _TRUNCATE truncates without invoking the handler; otherwise overflow -> buffer[0]=0, ERANGE, handler

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile f_s = "%s";
static const char *volatile s_abcdef = "abcdef";
TC_CASE(truncate)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = _snprintf_s(b, 4, _TRUNCATE, f_s, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate_fits)
{
    char b[16]; int n; fill(b); tc_iph_count = 0;
    n = _snprintf_s(b, 8, _TRUNCATE, f_s, s_abcdef);
    CHECK_EQ_I(n, 6); CHECK_STR(b, "abcdef"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(overflow_no_truncate)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = _snprintf_s(b, 4, 10, f_s, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(size0)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = _snprintf_s(b, 0, _TRUNCATE, f_s, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 'Z');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(truncate); TC_RUN(truncate_fits); TC_RUN(overflow_no_truncate); TC_RUN(size0);
    return tc_end();
}
