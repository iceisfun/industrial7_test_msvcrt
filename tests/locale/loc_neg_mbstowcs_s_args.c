/* TC
 * id:    loc_neg_mbstowcs_s_args
 * area:  locale
 * funcs: mbstowcs_s _mbstowcs_s_l
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mbstowcs_s (error conditions table), Parameter validation
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

errno_t __cdecl mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);
errno_t __cdecl _mbstowcs_s_l(size_t *, wchar_t *, size_t, const char *, size_t, _locale_t);

static char hello[] = "hello";

TC_CASE(wcstr_null_nonzero_size)
{
    size_t n = 99; errno_t r; int c0 = tc_iph_count;
    errno = 0; r = mbstowcs_s(&n, NULL, 8, hello, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(mbstr_null)
{
    wchar_t w[8]; size_t n = 99; errno_t r; int c0 = tc_iph_count; int i;
    for (i = 0; i < 8; i++) w[i] = 0x5a5a;
    errno = 0; r = mbstowcs_s(&n, w, 8, NULL, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(size_zero_null_dest_ok)
{
    size_t n = 99; errno_t r; int c0 = tc_iph_count;
    errno = 0; r = mbstowcs_s(&n, NULL, 0, hello, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, c0);
}
TC_CASE(iph_counts)
{
    wchar_t w[8]; size_t n = 99; int c0 = tc_iph_count;
    mbstowcs_s(&n, NULL, 8, hello, 5);
    mbstowcs_s(&n, w, 8, NULL, 5);
    _mbstowcs_s_l(&n, NULL, 8, hello, 5, NULL);
    CHECK_EQ_I(tc_iph_count - c0, 3);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(wcstr_null_nonzero_size); TC_RUN(mbstr_null);
    TC_RUN(size_zero_null_dest_ok); TC_RUN(iph_counts);
    return tc_end();
}
