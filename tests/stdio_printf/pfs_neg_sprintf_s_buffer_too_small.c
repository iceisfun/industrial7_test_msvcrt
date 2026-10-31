/* TC
 * id:    pfs_neg_sprintf_s_buffer_too_small
 * area:  stdio_printf
 * funcs: sprintf_s

 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: on overflow buffer[0] set to 0, invalid parameter handler invoked, errno ERANGE

 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile f_s = "%s";
static const char *volatile f_ls = "%ls";
static const char *volatile s_abcdef = "abcdef";
static const wchar_t *volatile w_abcdef = L"abcdef";
TC_CASE(too_long)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, f_s, s_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(needs_nul)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, "abcd" + (s_abcdef[0] != 'a'));
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
TC_CASE(len_plus_one_ok)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 5, f_s, "abcd");
    CHECK_EQ_I(n, 4); CHECK_STR(b, "abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_source_too_long)
{
    char b[16]; int n; fill(b); errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 4, f_ls, w_abcdef);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK(b[0] == 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(too_long); TC_RUN(needs_nul); TC_RUN(len_plus_one_ok); TC_RUN(wide_source_too_long);
    return tc_end();
}
