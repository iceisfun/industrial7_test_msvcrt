/* TC
 * id:    loc_neg_wcrtomb_s_args
 * area:  locale
 * funcs: 
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcrtomb_s error table
 * note:  Invalid-parameter handler is counted.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

TC_DECL_PROC(errno_t, wcrtomb_s, (size_t *, char *, size_t, wchar_t, void *));
static volatile wchar_t v_wc = L'A';

TC_CASE(null_buf_nonzero_size)
{
    size_t n = 99; errno_t e; int c0;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    rst(); c0 = tc_iph_count; errno = 0;
    e = tc_p_wcrtomb_s(&n, NULL, 8, v_wc, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
}
TC_CASE(nonnull_size_zero)
{
    char b[4]; size_t n = 99; errno_t e; int c0;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    rst(); c0 = tc_iph_count; errno = 0; memset(b, 0x55, sizeof b);
    e = tc_p_wcrtomb_s(&n, b, 0, v_wc, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
    CHECK_EQ_I((unsigned char)b[0], 0x55);
}
TC_CASE(null_buf_zero_size_ok)
{
    size_t n = 99; errno_t e; int c0;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    rst(); c0 = tc_iph_count;
    e = tc_p_wcrtomb_s(&n, NULL, 0, v_wc, PST);
    CHECK_EQ_I(e, 0); CHECK_EQ_I(tc_iph_count - c0, 0);
}
TC_CASE(iph_counts)
{
    char b[4]; size_t n = 99; int c0;
    if (!tc_p_wcrtomb_s || !tc_iph_install()) TC_SKIP("n/a");
    rst(); c0 = tc_iph_count;
    tc_p_wcrtomb_s(&n, NULL, 8, v_wc, PST);
    tc_p_wcrtomb_s(&n, b, 0, v_wc, PST);
    tc_p_wcrtomb_s(&n, NULL, 0, v_wc, PST);
    CHECK_EQ_I(tc_iph_count - c0, 2);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(null_buf_nonzero_size); TC_RUN(nonnull_size_zero); TC_RUN(null_buf_zero_size_ok); TC_RUN(iph_counts);
    return tc_end();
}
