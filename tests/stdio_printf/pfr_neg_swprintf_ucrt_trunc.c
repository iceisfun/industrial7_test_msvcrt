/* TC
 * id:    pfr_neg_swprintf_ucrt_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf, vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf (negative value if n or more wide characters were requested; output NUL terminated)
 * note:  Truncation is not a parameter error: no invalid-parameter handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int vcall(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(swprintf_trunc)
{
    wchar_t b[8]; int r;
    b[0]=b[1]=b[2]=b[3]=b[4]=L'Q';
    tc_iph_count = 0;
    r = swprintf(b, 4, L"%ls", L"abcdef");
    CHECK(r < 0);
    CHECK_WSTR(b, L"abc");
    CHECK_EQ_I(b[3], 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(vswprintf_trunc)
{
    wchar_t b[8]; int r;
    b[0]=b[1]=b[2]=b[3]=b[4]=L'Q';
    tc_iph_count = 0;
    r = vcall(b, 4, L"%ls", L"abcdef");
    CHECK(r < 0);
    CHECK_WSTR(b, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(n_is_one)
{
    wchar_t b[4]; int r;
    b[0] = L'Q'; b[1] = L'R';
    tc_iph_count = 0;
    r = swprintf(b, 1, L"%ls", L"abc");
    CHECK(r < 0);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(b[1], L'R');
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(swprintf_trunc); TC_RUN(vswprintf_trunc); TC_RUN(n_is_one);
    return tc_end();
}
