/* TC
 * id:    pfw_probe_stdio_common_nullterm_flag
 * area:  stdio_printf
 * funcs: 
 * procs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_stdio_config.h option bits
 * note:  PROBE: __stdio_common_vswprintf option bit 0x1 (_CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION): records buffer contents and return when output exactly fills count (L"abcd", count 4), with options 0 and 1. Only buf[0..2]==abc is asserted.
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>

typedef int (__cdecl *vsw_t)(unsigned __int64, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;

static int call(unsigned __int64 opt, wchar_t *b, size_t cnt, const wchar_t *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt); r = p_vsw(opt, b, cnt, fmt, NULL, ap); va_end(ap); return r;
}
static void note(const char *k, long long v) { char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(exact_fit_with_bit)
{
    wchar_t b[8]; const wchar_t *volatile s = L"abcd"; int i, r0, r1;
    if (!p_vsw) TC_SKIP("not exported");
    for (i = 0; i < 8; i++) b[i] = L'Z';
    r0 = call(0, b, 4, L"%ls", s);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    note("opt0_ret", r0); note("opt0_b3", b[3]); note("opt0_b4", b[4]);
    for (i = 0; i < 8; i++) b[i] = L'Z';
    r1 = call(1, b, 4, L"%ls", s);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    note("opt1_ret", r1); note("opt1_b3", b[3]); note("opt1_b4", b[4]);
}
int main(void) { tc_begin(); p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf"); TC_RUN(exact_fit_with_bit); return tc_end(); }
