/* TC
 * id:    pfr_neg_probe_sprintf_s_size0
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf_s (parameter validation)
 * note:  PROBE: sprintf_s(valid_buf, 0, "x") fails with -1, EINVAL, handler called once (sizeOfBuffer > 0 is validated like a NULL buffer); ERANGE is the alternative.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;

/* msvcr80's import library lacks the _s functions: resolve non-UCRT targets at run time (ucrtbase has them inline). */
#if TC_CRT_VER < 140
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
#endif
TC_CASE(valid_buffer_size0)
{
    char b[4]; volatile size_t z = 0; int r;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b); tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, z, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(valid_buffer_size0);
    return tc_end();
}
