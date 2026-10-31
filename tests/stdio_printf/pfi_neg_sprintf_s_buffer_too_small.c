/* TC
 * id:    pfi_neg_sprintf_s_buffer_too_small
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, Security Features in the CRT
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#if TC_CRT_VER >= 140
/* ucrtbase exports no sprintf_s symbol; the header inline goes through __stdio_common_vsprintf_s */
#define SS(...) sprintf_s(__VA_ARGS__)
#define HAVE_SS 1
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define SS(...) (tc_p_sprintf_s ? tc_p_sprintf_s(__VA_ARGS__) : -99)
#define HAVE_SS (tc_p_sprintf_s != NULL)
#endif

TC_CASE(too_small)
{
    char b[8]; int n; volatile int v = 12345;
    CHECK(HAVE_SS); if (!HAVE_SS || !tc_iph_install()) return;
    b[0] = 'x'; errno = 0; tc_iph_count = 0;
    n = SS(b, 4, "%d", v);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_boundary)
{
    char b[8]; int n; volatile int v = 12345;
    if (!HAVE_SS || !tc_iph_install()) return;
    tc_iph_count = 0; errno = 0;
    n = SS(b, 6, "%d", v);          /* len+1: fits */
    CHECK_EQ_I(n, 5); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 0);
    b[0] = 'x'; errno = 0;
    n = SS(b, 5, "%d", v);          /* size == len: no room for NUL */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(too_small); TC_RUN(size_boundary);
    return tc_end();
}
