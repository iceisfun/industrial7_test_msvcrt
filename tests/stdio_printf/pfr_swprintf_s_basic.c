/* TC
 * id:    pfr_swprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: swprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: returns number of wide characters stored, not counting the terminating null
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


TC_CASE(exported) { CHECK(tc_p_swprintf_s != NULL); }
TC_CASE(fit)
{
    wchar_t b[16]; volatile size_t n = 16; int r;
    r = tc_p_swprintf_s(b, n, L"%ls=%d", L"ab", 5);
    CHECK_EQ_I(r, 4);
    CHECK_WSTR(b, L"ab=5");
    CHECK_EQ_I(b[4], 0);
}
TC_CASE(exact_size)
{
    wchar_t b[8]; volatile size_t n = 4; int r;
    r = tc_p_swprintf_s(b, n, L"%ls", L"abc");
    CHECK_EQ_I(r, 3);
    CHECK_WSTR(b, L"abc");
}
TC_CASE(empty)
{
    wchar_t b[4]; volatile size_t n = 1; int r;
    b[0] = L'Z';
    r = tc_p_swprintf_s(b, n, L"%ls", L"");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}

int main(void) { tc_begin(); LOADP(swprintf_s); TC_RUN(exported); TC_RUN(fit); TC_RUN(exact_size); TC_RUN(empty); return tc_end(); }
