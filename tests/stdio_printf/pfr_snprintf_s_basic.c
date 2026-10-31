/* TC
 * id:    pfr_snprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: count is max chars to store excluding NUL; returns number of bytes written (not counting NUL) when output fits
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


TC_CASE(exported) { CHECK(tc_p__snprintf_s != NULL); }
TC_CASE(count_below_size)
{
    char b[10]; volatile size_t n = 10, c = 9; int r, i;
    for (i = 0; i < 10; i++) b[i] = 'Z';
    r = tc_p__snprintf_s(b, n, c, "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
}
TC_CASE(count_equals_len)
{
    char b[10]; volatile size_t n = 10, c = 3; int r;
    r = tc_p__snprintf_s(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(b[3], 0);
}
TC_CASE(count_larger_than_len)
{
    char b[10]; volatile size_t n = 10, c = 8; int r;
    r = tc_p__snprintf_s(b, n, c, "%d", 4242);
    CHECK_EQ_I(r, 4);
    CHECK_STR(b, "4242");
}

int main(void) { tc_begin(); LOADP(_snprintf_s); TC_RUN(exported); TC_RUN(count_below_size); TC_RUN(count_equals_len); TC_RUN(count_larger_than_len); return tc_end(); }
