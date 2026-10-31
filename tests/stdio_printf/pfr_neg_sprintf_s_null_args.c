/* TC
 * id:    pfr_neg_sprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s (buffer NULL or format NULL: invalid parameter handler, EINVAL, return -1)
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
TC_CASE(null_buffer_size0)
{
    char *volatile nb = NULL; volatile size_t z = 0; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(nb, z, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_nonzero_size)
{
    char *volatile nb = NULL; volatile size_t n = 8; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(nb, n, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(null_buffer_size0); TC_RUN(null_buffer_nonzero_size); TC_RUN(null_format);
    return tc_end();
}
