/* TC
 * id:    pfr_neg_sprintf_s_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, sprintf_s_l (buffer too small: invalid parameter handler, ERANGE, buffer[0] = 0, return -1)
 * note:  size == strlen is too small because the terminating NUL does not fit.
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
static const char * volatile v_s = "abcdef";

TC_CASE(small4)
{
    char b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b); tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 4, "%s", v_s);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_equals_len)
{
    char b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b); tc_iph_count = 0; errno = 0;
    r = sprintf_s(b, 6, "%s", v_s);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_len_plus_1_ok)
{
    char b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b); tc_iph_count = 0;
    r = sprintf_s(b, 7, "%s", v_s);
    CHECK_EQ_I(r, 6); CHECK_STR(b, "abcdef"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(small4); TC_RUN(size_equals_len); TC_RUN(size_len_plus_1_ok);
    return tc_end();
}
