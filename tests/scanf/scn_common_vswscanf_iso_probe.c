/* TC
 * id:    scn_common_vswscanf_iso_probe
 * area:  scanf
 * funcs: __stdio_common_vswscanf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _CRT_STDIO_ISO_WIDE_SPECIFIERS; C11 7.29.2.2 (%s/%c in wscanf use char*)
 * note:  PROBE: with options = 0 (no LEGACY_WIDE_SPECIFIERS bit) swscanf is expected to follow ISO C: %s and %c store narrow
 *        char, %ls/%lc store wchar_t. The docs describe this for programs defining _CRT_STDIO_ISO_WIDE_SPECIFIERS; that the
 *        define results in options == 0 is believed but not proven.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cw(unsigned long long opt, const wchar_t *in, const wchar_t *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
    r = __stdio_common_vswscanf(opt, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap); return r;
}
TC_CASE(s_narrow)  { char b[8]; int n; n = cw(0, L"abc", L"%s", b); CHECK_EQ_I(n, 1); CHECK_STR(b, "abc"); }
TC_CASE(c_narrow)  { char b[2]; int n; b[0] = b[1] = 'Z'; n = cw(0, L"k", L"%c", b); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'k'); CHECK_EQ_I(b[1], 'Z'); }
TC_CASE(ls_wide)   { wchar_t w[8]; int n; n = cw(0, L"wide", L"%ls", w); CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"wide"); }
TC_CASE(lc_wide)   { wchar_t w[2]; int n; w[0] = w[1] = L'Z'; n = cw(0, L"m", L"%lc", w); CHECK_EQ_I(n, 1); CHECK_EQ_I(w[0], L'm'); CHECK_EQ_I(w[1], L'Z'); }
int main(void)
{
    tc_begin();
    TC_RUN(s_narrow); TC_RUN(c_narrow); TC_RUN(ls_wide); TC_RUN(lc_wide);
    return tc_end();
}
