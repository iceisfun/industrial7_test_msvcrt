/* TC
 * id:    pfi_neg_sprintf_s_zero_size
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: sizeOfBuffer==0 is an invalid parameter
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

TC_CASE(zero_size)
{
    char b[4]; int n; volatile int v = 1;
    CHECK(HAVE_SS); if (!HAVE_SS || !tc_iph_install()) return;
    b[0] = 'x'; b[1] = 'y'; errno = 0; tc_iph_count = 0;
    n = SS(b, 0, "%d", v);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(b[0] == 'x' && b[1] == 'y');
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(zero_size);
    return tc_end();
}
