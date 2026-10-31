/* TC
 * id:    pfu_pct_n_enabled_probe_ucrt
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_printf_count_output; corecrt_stdio_config.h (_CRT_INTERNAL_PRINTF_* option bits)
 * note:  PROBING: interaction of the legacy _set_printf_count_output global with the UCRT secure %n handling, called through __stdio_common_vsprintf_s with options==0 (what the inline sprintf_s passes). Expectation (low confidence): after set(1) the %n conversion is honoured (return 3, n==3); after restoring 0 it is rejected again. The setting is restored to 0 in every case. The default-rejected case lives in pfu_neg_pct_n_sprintf_s.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
TC_DECL_PROC(int, _get_printf_count_output, (void));

static int cv(char *b, size_t n, unsigned long long opt, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf_s(opt, b, n, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(setter_roundtrip)
{
    CHECK(tc_p__set_printf_count_output && tc_p__get_printf_count_output);
    if (!tc_p__set_printf_count_output) return;
    CHECK_EQ_I(tc_p__set_printf_count_output(1), 0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 1);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(tc_p__get_printf_count_output(), 0);
}
TC_CASE(enabled_sprintf_s)
{
    char b[32]; int n = -1, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(enabled_direct_options0)
{
    char b[32]; int n = -1, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0;
    r = cv(b, 32, 0, (const char *)f, &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
}
TC_CASE(rejected_after_restore)
{
    char b[32]; int n = 55, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_p__set_printf_count_output(0);
    tc_iph_count = 0;
    r = sprintf_s(b, 32, (const char *)f, &n);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 55);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_set_printf_count_output); TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(setter_roundtrip); TC_RUN(enabled_sprintf_s); TC_RUN(enabled_direct_options0); TC_RUN(rejected_after_restore);
    return tc_end();
}
