/* TC
 * id:    pfr_neg_vsprintf_s_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vsprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vsprintf_s (ERANGE too small, EINVAL NULL buffer/format)
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
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
#define vsprintf_s tc_p_vsprintf_s
#endif
static int vs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(small)
{
    char b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    memset(b, 'X', sizeof b); tc_iph_count = 0; errno = 0;
    r = vs(b, 4, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(nb, 8, "%s", "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ok_after_errors)
{
    char b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = vs(b, sizeof b, "%d", 42);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "42"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(vsprintf_s);
    if (!tc_p_vsprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(small); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(ok_after_errors);
    return tc_end();
}
