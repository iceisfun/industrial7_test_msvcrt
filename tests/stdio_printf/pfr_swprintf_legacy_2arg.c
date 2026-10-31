/* TC
 * id:    pfr_swprintf_legacy_2arg
 * area:  stdio_printf
 * funcs:
 * procs: _swprintf
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, swprintf, _swprintf (non-conforming form _swprintf(buffer, format, ...)): returns number of wide chars written, not counting NUL
 * note:  msvcrt.dll and msvcr80 not listed: not sure the DLL exports the name _swprintf (msvcrt.dll exports the 2-arg form as swprintf).
 *        On ucrtbase _swprintf is a header inline over __stdio_common_vswprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_DECL_PROC(int, _swprintf, (wchar_t *, const wchar_t *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


TC_CASE(exported) { CHECK(tc_p__swprintf != NULL); }
TC_CASE(int_arg)
{
    wchar_t b[32];
    int r = tc_p__swprintf(b, L"%d", 42);
    CHECK_EQ_I(r, 2);
    CHECK_WSTR(b, L"42");
}
TC_CASE(long_output)
{
    wchar_t b[256]; wchar_t big[101]; int i, r;
    for (i = 0; i < 100; i++) big[i] = L'x';
    big[100] = 0;
    r = tc_p__swprintf(b, L"[%ls]", big);
    CHECK_EQ_I(r, 102);
    CHECK_EQ_I(b[0], L'[');
    CHECK_EQ_I(b[101], L']');
    CHECK_EQ_I(b[102], 0);
}
TC_CASE(empty)
{
    wchar_t b[8]; int r;
    b[0] = L'Z';
    r = tc_p__swprintf(b, L"%ls", L"");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}

int main(void) { tc_begin(); LOADP(_swprintf); TC_RUN(exported); TC_RUN(int_arg); TC_RUN(long_output); TC_RUN(empty); return tc_end(); }
