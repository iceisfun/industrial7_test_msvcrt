/* TC
 * id:    scn_common_vswscanf_wide_specifiers
 * area:  scanf
 * funcs: __stdio_common_vswscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Format specification syntax: scanf and wscanf functions" (%s/%c in wscanf are wide by default; h/l size prefixes), _CRT_STDIO_ISO_WIDE_SPECIFIERS
 * note:  options = 2 (_CRT_INTERNAL_SCANF_LEGACY_WIDE_SPECIFIERS) selects the legacy Microsoft meaning (%s/%c wide, %hs/%hc narrow) documented for wscanf; the test passes it explicitly and does not claim what the default inline swscanf passes.
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

TC_CASE(s_is_wide)
{
    wchar_t w[8]; int n;
    n = cw(2, L"hello", L"%s", w);
    CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"hello");
}
TC_CASE(c_is_wide)
{
    wchar_t w[2]; int n;
    w[0] = w[1] = L'Z';
    n = cw(2, L"xy", L"%c", w);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(w[0], L'x'); CHECK_EQ_I(w[1], L'Z');
}
TC_CASE(hs_narrow)
{
    char b[8]; int n;
    n = cw(2, L"narrow", L"%hs", b);
    CHECK_EQ_I(n, 1); CHECK_STR(b, "narrow");
}
TC_CASE(hc_narrow)
{
    char b[2]; int n;
    b[0] = b[1] = 'Z';
    n = cw(2, L"q", L"%hc", b);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'q'); CHECK_EQ_I(b[1], 'Z');
}
TC_CASE(ls_wide)
{
    wchar_t w[8]; int n;
    n = cw(2, L"wide", L"%ls", w);
    CHECK_EQ_I(n, 1); CHECK_WSTR(w, L"wide");
}
TC_CASE(mixed)
{
    wchar_t w[8]; char b[8]; int v = 0, n;
    n = cw(2, L"7 ab cd", L"%d %s %hs", &v, w, b);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(v, 7); CHECK_WSTR(w, L"ab"); CHECK_STR(b, "cd");
}
int main(void)
{
    tc_begin();
    TC_RUN(s_is_wide); TC_RUN(c_is_wide); TC_RUN(hs_narrow); TC_RUN(hc_narrow); TC_RUN(ls_wide); TC_RUN(mixed);
    return tc_end();
}
