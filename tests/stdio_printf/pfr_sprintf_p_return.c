/* TC
 * id:    pfr_sprintf_p_return
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p, _scprintf_p
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs printf_p positional parameters, _sprintf_p, _scprintf_p: return chars written / count, excluding NUL
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _sprintf_p, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _scprintf_p, (const char *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


TC_CASE(exported) { CHECK(tc_p__sprintf_p != NULL); CHECK(tc_p__scprintf_p != NULL); }
TC_CASE(positional)
{
    char b[32]; volatile size_t n = 32; int r;
    r = tc_p__sprintf_p(b, n, "%2$s %1$s", "a", "b");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "b a");
}
TC_CASE(count)
{
    CHECK_EQ_I(tc_p__scprintf_p("%2$s %1$s", "a", "b"), 3);
    CHECK_EQ_I(tc_p__scprintf_p("%1$s%1$s", "xy"), 4);
}
TC_CASE(exact_size)
{
    char b[8]; volatile size_t n = 4; int r;
    r = tc_p__sprintf_p(b, n, "%2$s %1$s", "a", "b");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "b a");
}

int main(void) { tc_begin(); LOADP(_sprintf_p); LOADP(_scprintf_p); TC_RUN(exported); TC_RUN(positional); TC_RUN(count); TC_RUN(exact_size); return tc_end(); }
