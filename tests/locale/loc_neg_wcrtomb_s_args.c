/* TC
 * id:    loc_neg_wcrtomb_s_args
 * area:  locale
 * funcs: 
 * procs: wcrtomb_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcrtomb_s error table
 * note:  Invalid-parameter handler is counted.
 *        Win10 triage (class B): ucrtbase 19045 accepts a non-NULL buffer with sizeInBytes == 0 (writes the byte, returns 0); only the NULL-buffer/size>0
 *        case is an EINVAL. Observed on Windows 10 22H2 build 19045.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [class B] deviates from the runtime-constraint rules of C11 Annex K (K.3.6.x / K.3.9.3.x) that the MS docs mirror; expectation = Windows behavior.
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
    /* Win10 22H2 ucrtbase (observed): sizeInBytes == 0 with a non-NULL buffer is NOT rejected: returns 0, *pReturnValue == 1 and 'A' is written to b[0]
     * (deviates from the MS docs EINVAL row; the declared size is ignored). */
    CHECK_EQ_I(e, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count - c0, 0);
    CHECK_EQ_U(n, 1); CHECK_EQ_I((unsigned char)b[0], 'A');
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
    CHECK_EQ_I(tc_iph_count - c0, 1);   /* only (NULL buffer, size 8) is rejected; see nonnull_size_zero */
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcrtomb_s);
    TC_RUN(null_buf_nonzero_size); TC_RUN(nonnull_size_zero); TC_RUN(null_buf_zero_size_ok); TC_RUN(iph_counts);
    return tc_end();
}
