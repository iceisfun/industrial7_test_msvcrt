/* TC
 * id:    pfl_neg_printf_n_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_l _create_locale _free_locale
 * procs: _get_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output: %n support is disabled by default
 * note:  Valid only where the process has the default (0) setting; case default_is_zero guards the rest.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static _locale_t loc;
static int def_zero;

TC_CASE(default_is_zero)
{
    CHECK(tc_p__get_printf_count_output != NULL);
    if (tc_p__get_printf_count_output) { def_zero = tc_p__get_printf_count_output() == 0; CHECK(def_zero); }
}
TC_CASE(pct_n_iph)
{
    char b[32]; int n = 12345; volatile const char *f = "ab%ncd";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    _sprintf_l(b, (const char *)f, loc, &n);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(pct_n_ret_minus1)
{
    char b[32]; int n = 12345, r; volatile const char *f = "ab%ncd";
    if (!def_zero) TC_SKIP("%n enabled");
    r = _sprintf_l(b, (const char *)f, loc, &n);
    CHECK_EQ_I(r, -1);
}
TC_CASE(target_unchanged)
{
    char b[32]; int n = 12345; volatile const char *f = "ab%ncd";
    if (!def_zero) TC_SKIP("%n enabled");
    _sprintf_l(b, (const char *)f, loc, &n);
    CHECK_EQ_I(n, 12345);
}
TC_CASE(errno_einval)
{
    char b[32]; int n = 1; volatile const char *f = "ab%ncd";
    if (!def_zero) TC_SKIP("%n enabled");
    errno = 0;
    _sprintf_l(b, (const char *)f, loc, &n);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(default_is_zero); TC_RUN(pct_n_iph); TC_RUN(pct_n_ret_minus1); TC_RUN(target_unchanged); TC_RUN(errno_einval);
    _free_locale(loc);
    return tc_end();
}
