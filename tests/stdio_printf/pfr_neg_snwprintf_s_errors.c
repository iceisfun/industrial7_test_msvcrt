/* TC
 * id:    pfr_neg_snwprintf_s_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _snwprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s (count > size too small: ERANGE; _TRUNCATE: truncate, -1, no handler; NULL: EINVAL)
 * note:  errno after _TRUNCATE truncation (STRUNCATE) is not asserted.
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
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define _snwprintf_s tc_p__snwprintf_s
#endif
static const wchar_t * volatile v_w = L"abcdef";

TC_CASE(count_gt_size_small)
{
    wchar_t b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    b[0] = L'X'; tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(b, 4, 10, L"%ls", v_w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(truncate)
{
    wchar_t b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    b[0] = L'X'; tc_iph_count = 0;
    r = _snwprintf_s(b, 4, (size_t)-1, L"%ls", v_w);
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(nb, 8, 4, L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snwprintf_s(b, 8, 4, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(_snwprintf_s);
    if (!tc_p__snwprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(count_gt_size_small); TC_RUN(truncate); TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
