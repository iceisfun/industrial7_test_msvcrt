/* TC
 * id:    pfo_neg_vsprintf_n_specifier
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * procs: _set_invalid_parameter_handler _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output, printf type field n: %n support is disabled by default; when disabled, printf-family calls invoke the invalid parameter handler
 * note:  Each case restores the count-output flag to 0 before returning.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

static int cv(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static int cvs(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(default_disabled)
{
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(sprintf_n_disabled)
{
    char b[32]; int n = 0x5a5a5a5a, r;
    tc_p__set_printf_count_output(0); tc_iph_count = 0;
    r = cv(b, sizeof b, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sprintf_s_n_disabled)
{
    char b[32]; int n = 0x5a5a5a5a, r;
    tc_p__set_printf_count_output(0); tc_iph_count = 0;
    r = cvs(b, sizeof b, "ab%n", &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 0x5a5a5a5a);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(enabled_stores_count)
{
    char b[32]; int n = -1, r, prev;
    tc_iph_count = 0;
    prev = tc_p__set_printf_count_output(1);
    CHECK_EQ_I(prev, 0);
    r = cv(b, sizeof b, "abc%n", &n);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
    n = -1;
    r = cvs(b, sizeof b, "abcd%n", &n);
    CHECK_EQ_I(r, 4);
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_p__set_printf_count_output(0), 1);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    if (!tc_p__set_printf_count_output || !tc_p__get_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__procs missing\n"); return 2; }
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(default_disabled); TC_RUN(sprintf_n_disabled); TC_RUN(sprintf_s_n_disabled); TC_RUN(enabled_stores_count);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
