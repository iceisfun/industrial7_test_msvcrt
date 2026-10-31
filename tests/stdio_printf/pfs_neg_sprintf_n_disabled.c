/* TC
 * id:    pfs_neg_sprintf_n_disabled
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output: %n support is off by default since VS2005; %n then invokes the invalid parameter handler

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static const char *volatile f_n = "ab%n";
static const wchar_t *volatile wf_n = L"ab%n";
TC_CASE(default_off) { CHECK(tc_p__get_printf_count_output != NULL); if (tc_p__get_printf_count_output) CHECK_EQ_I(tc_p__get_printf_count_output(), 0); }
TC_CASE(sprintf_n)
{
    char b[16]; volatile int n = 12345; int r; b[0] = 'Z'; errno = 0; tc_iph_count = 0;
    r = sprintf(b, f_n, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
}
TC_CASE(snwprintf_n)
{
    wchar_t w[16]; volatile int n = 12345; int r; w[0] = L'Z'; errno = 0; tc_iph_count = 0;
    r = _snwprintf(w, 8, wf_n, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_get_printf_count_output); CHECK(tc_iph_install());
    TC_RUN(default_off); TC_RUN(sprintf_n); TC_RUN(snwprintf_n);
    return tc_end();
}
