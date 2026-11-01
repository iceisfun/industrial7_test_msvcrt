/* TC
 * id:    loc_neg_wcstombs_s_args
 * area:  locale
 * funcs: wcstombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s (error conditions table), Parameter validation
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

errno_t __cdecl wcstombs_s(size_t *, char *, size_t, const wchar_t *, size_t);


static wchar_t w_hello[] = L"hello";

TC_CASE(mbstr_null_nonzero_size)
{
    size_t n = 99; errno_t r; int c0 = tc_iph_count;
    errno = 0; r = wcstombs_s(&n, NULL, 8, w_hello, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(wcstr_null)
{
    char b[8]; size_t n = 99; errno_t r; int c0 = tc_iph_count;
    { int i; for (i = 0; i < 8; i++) b[i] = 'Z'; }
    errno = 0; r = wcstombs_s(&n, b, 8, NULL, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(iph_counts)
{
    char b[8]; size_t n = 99; int c0 = tc_iph_count;
    wcstombs_s(&n, NULL, 8, w_hello, 5);
    wcstombs_s(&n, b, 8, NULL, 5);
    CHECK_EQ_I(tc_iph_count - c0, 2);   /* two bad calls in this case; the old expectation of 3 miscounted (c0 is sampled at the start of this case) */
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(mbstr_null_nonzero_size); TC_RUN(wcstr_null);
    TC_RUN(iph_counts);
    return tc_end();
}
