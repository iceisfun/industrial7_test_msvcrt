/* TC
 * id:    pfs_neg_sprintf_s_null_args
 * area:  stdio_printf
 * funcs: sprintf_s swprintf_s

 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: NULL buffer or format is an invalid parameter, errno EINVAL

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static char *volatile nb = NULL;
static wchar_t *volatile nw = NULL;
static const char *volatile nf = NULL;
static const wchar_t *volatile nwf = NULL;
static const char *volatile f_x = "x";
static const wchar_t *volatile wf_x = L"x";
TC_CASE(null_buffer)
{
    int n; errno = 0; tc_iph_count = 0;
    n = sprintf_s(nb, 10, f_x);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[10]; int n; b[0] = 'Z'; errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 10, nf);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_buffer)
{
    int n; errno = 0; tc_iph_count = 0;
    n = swprintf_s(nw, 10, wf_x);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_format)
{
    wchar_t w[10]; int n; w[0] = L'Z'; errno = 0; tc_iph_count = 0;
    n = swprintf_s(w, 10, nwf);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(wide_null_buffer); TC_RUN(wide_null_format);
    return tc_end();
}
