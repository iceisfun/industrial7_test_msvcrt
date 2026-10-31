/* TC
 * id:    pfs_neg_swprintf_s_buffer_too_small
 * area:  stdio_printf
 * funcs: swprintf_s

 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf_s: overflow -> buffer[0]=0, ERANGE, invalid parameter handler; size 0 -> EINVAL

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t *volatile f_s = L"%s";
static const wchar_t *volatile f_hs = L"%hs";
static const wchar_t *volatile w_abcdef = L"abcdef";
static const char *volatile s_abcdef = "abcdef";
TC_CASE(too_long)
{
    wchar_t w[16]; int n; fill(w); errno = 0; tc_iph_count = 0;
    n = swprintf_s(w, 4, f_s, w_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == 0);
}
TC_CASE(size0)
{
    wchar_t w[16]; int n; fill(w); errno = 0; tc_iph_count = 0;
    n = swprintf_s(w, 0, f_s, w_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == L'Z');
}
TC_CASE(narrow_source_too_long)
{
    wchar_t w[16]; int n; fill(w); errno = 0; tc_iph_count = 0;
    n = swprintf_s(w, 4, f_hs, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(w[0] == 0);
}
TC_CASE(fits)
{
    wchar_t w[16]; int n; fill(w); tc_iph_count = 0;
    n = swprintf_s(w, 7, f_hs, s_abcdef);
    CHECK_EQ_I(n, 6); CHECK_WSTR(w, L"abcdef"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(too_long); TC_RUN(size0); TC_RUN(narrow_source_too_long); TC_RUN(fits);
    return tc_end();
}
