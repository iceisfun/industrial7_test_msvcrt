/* TC
 * id:    pfr_neg_snwprintf_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf _vsnwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _vsnwprintf: count wide chars written, no NUL, returns -1 on overflow
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define G 0x5A5A
static int cv(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(snwprintf_trunc)
{
    wchar_t b[16]; int i, r; volatile const wchar_t *f = L"abcdef";
    for (i = 0; i < 16; i++) b[i] = G;
    r = _snwprintf(b, 3, (const wchar_t *)f);
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    CHECK_EQ_I(b[3], G);
}
TC_CASE(vsnwprintf_trunc)
{
    wchar_t b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = cv(b, 3, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    CHECK_EQ_I(b[3], G);
}
TC_CASE(snwprintf_exact_fit)
{
    wchar_t b[16]; int i, r;
    for (i = 0; i < 16; i++) b[i] = G;
    r = _snwprintf(b, 6, L"%ls", L"abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_EQ_I(b[5], L'f');
    CHECK_EQ_I(b[6], G);
}
TC_CASE(handler_not_invoked)
{
    wchar_t b[8]; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = _snwprintf(b, 2, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(snwprintf_trunc); TC_RUN(vsnwprintf_trunc); TC_RUN(snwprintf_exact_fit); TC_RUN(handler_not_invoked);
    return tc_end();
}
