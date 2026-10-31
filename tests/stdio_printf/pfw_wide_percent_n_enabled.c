/* TC
 * id:    pfw_wide_percent_n_enabled
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output / _get_printf_count_output (VS2005+): %n disabled by default
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

TC_CASE(get_default_0) { if (!tc_p__get_printf_count_output) TC_SKIP("no api"); CHECK_EQ_I(tc_p__get_printf_count_output(), 0); }
TC_CASE(set_1_get_1)
{
    if (!tc_p__set_printf_count_output) TC_SKIP("no api");
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);   /* returns previous setting */
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    tc_p__set_printf_count_output(0);
}
TC_CASE(percent_n_stores_wchar_count)
{
    wchar_t b[32]; int n = -1, r;
    if (!tc_p__set_printf_count_output) TC_SKIP("no api");
    tc_p__set_printf_count_output(1);
    r = _snwprintf(b, 32, L"abc%nde", &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(r, 5);
    CHECK_WSTR(b, L"abcde");
}
TC_CASE(restore_0)
{
    if (!tc_p__set_printf_count_output) TC_SKIP("no api");
    tc_p__set_printf_count_output(1);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(get_default_0); TC_RUN(set_1_get_1); TC_RUN(percent_n_stores_wchar_count); TC_RUN(restore_0);
    return tc_end();
}
