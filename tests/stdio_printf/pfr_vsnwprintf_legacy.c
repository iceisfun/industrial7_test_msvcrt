/* TC
 * id:    pfr_vsnwprintf_legacy
 * area:  stdio_printf
 * funcs:
 * procs: _vsnwprintf, _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _vsnwprintf: len < count -> returns len, appends L'\0'; len == count -> returns count, no terminator
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

TC_DECL_PROC(int, _vsnwprintf, (wchar_t *, size_t, const wchar_t *, va_list));
TC_DECL_PROC(int, _snwprintf, (wchar_t *, size_t, const wchar_t *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


static int cv(wchar_t *b, size_t n, const wchar_t *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = tc_p__vsnwprintf(b, n, f, ap); va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(tc_p__vsnwprintf != NULL); CHECK(tc_p__snwprintf != NULL); }
TC_CASE(fit)
{
    wchar_t b[16]; volatile size_t n = 16; int i, r;
    for (i = 0; i < 16; i++) b[i] = L'Z';
    r = tc_p__snwprintf(b, n, L"%ls-%d", L"abc", 12);
    CHECK_EQ_I(r, 6);
    CHECK_WSTR(b, L"abc-12");
    CHECK_EQ_I(b[6], 0);
}
TC_CASE(exact_fill)
{
    wchar_t b[8]; volatile size_t n = 5; int i, r;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    r = tc_p__snwprintf(b, n, L"%ls", L"abcde");
    CHECK_EQ_I(r, 5);
    CHECK(b[0] == L'a' && b[4] == L'e');
    CHECK(b[5] == L'Z');
}
TC_CASE(va_matches)
{
    wchar_t b[16], c[16]; volatile size_t n = 16; int i, r1, r2;
    for (i = 0; i < 16; i++) b[i] = c[i] = L'Z';
    r1 = tc_p__snwprintf(b, n, L"%d:%ls", 77, L"xy");
    r2 = cv(c, n, L"%d:%ls", 77, L"xy");
    CHECK_EQ_I(r1, 5);
    CHECK_EQ_I(r2, r1);
    CHECK_WSTR(b, c);
    CHECK_WSTR(c, L"77:xy");
}

int main(void) { tc_begin(); LOADP(_vsnwprintf); LOADP(_snwprintf); TC_RUN(exported); TC_RUN(fit); TC_RUN(exact_fill); TC_RUN(va_matches); return tc_end(); }
