/* TC
 * id:    pfq_sprintf_p_wide_mix_hs_ls
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf type field characters / size prefixes (w, l, h, I32, I64, I); z and t where supported (UCRT)
 * note:  Positional variants of the narrow-format size prefixes. z/t only asserted for ucrtbase.
 */
#include "tc.h"
#include "pfq_shim.h"

TC_CASE(ws_wide_string)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "[%1$ws][%2$ls]", L"wide", L"w2");
    CHECK_EQ_I(r, 10); CHECK_STR(b, "[wide][w2]");
}
TC_CASE(hc_lc_chars)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%1$hc%2$lc%3$c", 'a', (wint_t)L'b', 'c');
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc");
}
TC_CASE(hs_ls_strings)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%2$hs/%1$ls", L"W", "N");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "N/W");
}
TC_CASE(i32_i64)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%2$I64d %1$I32d", 123456, 5000000000LL);
    CHECK_EQ_I(r, 17); CHECK_STR(b, "5000000000 123456");
}
TC_CASE(ptr_sized)
{
    char b[64]; intptr_t v = -77;
    int r = pq_sprintf_p(b, sizeof b, "%1$Id", v);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "-77");
}
#ifdef _UCRT
TC_CASE(z_t_ucrt)
{
    char b[64]; size_t z = 123; ptrdiff_t t = -45;
    int r = pq_sprintf_p(b, sizeof b, "%2$zd|%1$td", t, z);
    CHECK_EQ_I(r, 7); CHECK_STR(b, "123|-45");
}
#endif
int main(void)
{
    tc_begin();
    TC_RUN(ws_wide_string); TC_RUN(hc_lc_chars); TC_RUN(hs_ls_strings); TC_RUN(i32_i64); TC_RUN(ptr_sized);
#ifdef _UCRT
    TC_RUN(z_t_ucrt);
#endif
    return tc_end();
}
