/* TC
 * id:    pfi_neg_swprintf_s_bad_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Type Field Characters (invalid type -> invalid parameter); %n disabled by default in the secure/UCRT printf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile f_badtype = L"%d %y";
static const wchar_t *volatile f_pct_n = L"%n";

TC_CASE(invalid_type_char)
{
    wchar_t b[32]; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 32, f_badtype, 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(percent_n)
{
    wchar_t b[32]; int cnt = 77; int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(b, 32, f_pct_n, &cnt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(cnt, 77);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(invalid_type_char); TC_RUN(percent_n);
    return tc_end();
}
