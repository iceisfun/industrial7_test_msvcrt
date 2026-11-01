/* TC
 * id:    pfu_pct_n_enabled_probe_ucrt
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s
 * procs: _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _set_printf_count_output; corecrt_stdio_config.h (_CRT_INTERNAL_PRINTF_* option bits)
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "enabled_sprintf_s r == 3 got=-1", "enabled_direct_options0 r == 3 got=-1"; zzp_probe4 "sprintf_s %n enabled: r=-1
 *        errno=22 iph=1 n=-1", "vss opt0 %n enabled: r=-1 iph=1", "sprintf %n enabled: r=3 n=3", "vs opt0 %n enabled: r=3 n=3"): after
 *        _set_printf_count_output(1) the NON-secure family (sprintf, __stdio_common_vsprintf options 0) honours %n (3, n==3, new case
 *        enabled_nonsecure_sprintf), but the SECURE family (sprintf_s via __stdio_common_vsprintf_s, options 0) still rejects %n (-1, EINVAL, handler
 *        once, n untouched). The original low-confidence guess for the secure path was wrong. setter_roundtrip and rejected_after_restore unchanged. Stable.
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
    CHECK_EQ_I(r, -1);     /* observed: the secure variant rejects %n even when enabled */
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(enabled_direct_options0)
{
    char b[32]; int n = -1, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0;
    r = cv(b, 32, 0, (const char *)f, &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, -1);     /* observed: __stdio_common_vsprintf_s rejects %n even when enabled */
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(enabled_nonsecure_sprintf)
{
    char b[32]; int n = -1, r; volatile const char *f = "abc%n";
    if (!tc_p__set_printf_count_output) return;
    tc_p__set_printf_count_output(1);
    tc_iph_count = 0;
    r = sprintf(b, (const char *)f, &n);
    tc_p__set_printf_count_output(0);
    CHECK_EQ_I(r, 3);      /* observed: the non-secure sprintf honours %n once enabled */
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(tc_iph_count, 0);
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
    TC_RUN(setter_roundtrip); TC_RUN(enabled_sprintf_s); TC_RUN(enabled_direct_options0); TC_RUN(enabled_nonsecure_sprintf); TC_RUN(rejected_after_restore);
    return tc_end();
}
