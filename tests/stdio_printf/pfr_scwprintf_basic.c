/* TC
 * id:    pfr_scwprintf_basic
 * area:  stdio_printf
 * funcs:
 * procs: _scwprintf, _vscwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf, _scprintf_l, _scwprintf_l: returns number of wide characters, not counting the terminating null
 * note:  resolved at run time (import libs lack these). msvcrt.dll not listed: unsure it exports _scwprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

TC_DECL_PROC(int, _scwprintf, (const wchar_t *, ...));
TC_DECL_PROC(int, _vscwprintf, (const wchar_t *, va_list));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif

static int vc(const wchar_t *f, ...)
{
    va_list ap; int r;
    if (!tc_p__vscwprintf) return -99;
    va_start(ap, f); r = tc_p__vscwprintf(f, ap); va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(tc_p__scwprintf != NULL); CHECK(tc_p__vscwprintf != NULL); }
TC_CASE(basic)
{
    if (!tc_p__scwprintf) TC_SKIP("missing");
    const wchar_t * volatile f = L"%ls %d"; const wchar_t * volatile s = L"ab";
    CHECK_EQ_I(tc_p__scwprintf(f, s, 5), 4);
}
TC_CASE(via_va_list)
{
    const wchar_t * volatile f = L"%ls %d"; const wchar_t * volatile s = L"ab";
    CHECK_EQ_I(vc(f, s, 5), 4);
    CHECK_EQ_I(vc(f, s, 12345), 8);
}
TC_CASE(empty)
{
    if (!tc_p__scwprintf) TC_SKIP("missing");
    const wchar_t * volatile f = L"";
    CHECK_EQ_I(tc_p__scwprintf(f), 0);
    CHECK_EQ_I(vc(f), 0);
}
TC_CASE(counts_wchars_not_bytes)
{
    if (!tc_p__scwprintf) TC_SKIP("missing");
    const wchar_t * volatile s = L"\x4e2d\x6587";   /* two CJK chars */
    CHECK_EQ_I(tc_p__scwprintf(L"%ls", s), 2);
    CHECK_EQ_I(vc(L"%ls", s), 2);
    CHECK_EQ_I(tc_p__scwprintf(L"%ls!", s), 3);
}

int main(void) { tc_begin(); LOADP(_scwprintf); LOADP(_vscwprintf); TC_RUN(exported); TC_RUN(basic); TC_RUN(via_va_list); TC_RUN(empty); TC_RUN(counts_wchars_not_bytes); return tc_end(); }
