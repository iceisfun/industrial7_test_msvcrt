/* TC
 * id:    pfr_sprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l: returns number of bytes stored not counting the NUL; buffer must hold len+1
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


TC_CASE(exported) { CHECK(tc_p_sprintf_s != NULL); }
TC_CASE(exact_size)
{
    char b[8]; volatile size_t n = 4; int r;
    r = tc_p_sprintf_s(b, n, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
}
TC_CASE(large_size)
{
    char b[64]; volatile size_t n = 64; int r;
    r = tc_p_sprintf_s(b, n, "%s", "hello");
    CHECK_EQ_I(r, 5);
    CHECK_STR(b, "hello");
}
TC_CASE(empty)
{
    char b[4]; volatile size_t n = 1; int r;
    b[0] = 'Z';
    r = tc_p_sprintf_s(b, n, "%s", "");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(mixed)
{
    char b[32]; volatile size_t n = 32; int r;
    r = tc_p_sprintf_s(b, n, "%d %s %d", 10, "mid", -3);
    CHECK_EQ_I(r, 9);
    CHECK_STR(b, "10 mid -3");
}

int main(void) { tc_begin(); LOADP(sprintf_s); TC_RUN(exported); TC_RUN(exact_size); TC_RUN(large_size); TC_RUN(empty); TC_RUN(mixed); return tc_end(); }
