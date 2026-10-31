/* TC
 * id:    pfr_neg_probe_sprintf_s_null_format_buf0
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf_s
 * note:  PROBE: after sprintf_s(b, n, NULL) fails with EINVAL, is b[0] set to 0? Docs state buffer[0]=0 for the too-small case; for NULL format we guess it is also zeroed.
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
TC_CASE(buf0_after_null_format)
{
    char b[8]; const char *volatile f = NULL;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b);
    sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(buf0_after_null_format);
    return tc_end();
}
