/* TC
 * id:    cv_neg_ultoa_s_errors
 * area:  conv
 * funcs: _ultoa_s _ui64toa_s _ltoa_s _ultow_s _ui64tow_s _ltow_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * note:  Buffer contents after failure and the exact too-small code (EINVAL per docs table vs ERANGE) are deliberately not asserted here.
 * ref:   MS docs _itoa_s, _itow_s family (EINVAL: NULL buffer, size 0, radix outside 2..36; too small buffer -> error + invalid-parameter handler)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile unsigned long v_ul = 4294967295UL;

TC_CASE(null_buffer)
{
    int n0 = tc_iph_count;
    CHECK_EQ_I(_ultoa_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_I(_ui64toa_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 2);
    CHECK_EQ_I(_ltoa_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 3);
    CHECK_EQ_I(_ultow_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 4);
    CHECK_EQ_I(_ui64tow_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 5);
    CHECK_EQ_I(_ltow_s(5, NULL, 10, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 6);
}
TC_CASE(size_zero)
{
    char b[8] = "zz"; wchar_t w[8] = L"zz"; int n0 = tc_iph_count;
    CHECK_EQ_I(_ultoa_s(5, b, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_I(_ui64toa_s(5, b, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 2);
    CHECK_EQ_I(_ltoa_s(5, b, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 3);
    CHECK_EQ_I(_ultow_s(5, w, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 4);
    CHECK_EQ_I(_ui64tow_s(5, w, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 5);
    CHECK_EQ_I(_ltow_s(5, w, 0, 10), EINVAL); CHECK_EQ_I(tc_iph_count - n0, 6);
    CHECK_STR(b, "zz");
}
TC_CASE(bad_radix_narrow)
{
    static const int radices[] = { 1, 37, 0, -2 };
    size_t i;
    for (i = 0; i < sizeof radices / sizeof radices[0]; i++) {
        char b[16]; int n0 = tc_iph_count; int r = radices[i];
        b[0] = 'z'; CHECK_EQ_I(_ultoa_s(5, b, sizeof b, r), EINVAL);
        b[0] = 'z'; CHECK_EQ_I(_ui64toa_s(5, b, sizeof b, r), EINVAL);
        b[0] = 'z'; CHECK_EQ_I(_ltoa_s(5, b, sizeof b, r), EINVAL);
        CHECK_EQ_I(tc_iph_count - n0, 3);
    }
}
TC_CASE(bad_radix_wide)
{
    static const int radices[] = { 1, 37, 0 };
    size_t i;
    for (i = 0; i < sizeof radices / sizeof radices[0]; i++) {
        wchar_t w[16]; int n0 = tc_iph_count; int r = radices[i];
        w[0] = L'z'; CHECK_EQ_I(_ultow_s(5, w, 16, r), EINVAL);
        w[0] = L'z'; CHECK_EQ_I(_ui64tow_s(5, w, 16, r), EINVAL);
        w[0] = L'z'; CHECK_EQ_I(_ltow_s(5, w, 16, r), EINVAL);
        CHECK_EQ_I(tc_iph_count - n0, 3);
    }
}
TC_CASE(too_small_ultoa)
{
    char b[16]; int n0 = tc_iph_count;
    b[0] = 'z';
    CHECK(_ultoa_s(v_ul, b, 5, 10) != 0);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    b[0] = 'z';
    CHECK(_ultoa_s(v_ul, b, 10, 10) != 0);   /* 10 digits need 11 with NUL */
}
TC_CASE(exact_fit_ultoa)
{
    char b[16]; int n0 = tc_iph_count;
    CHECK_EQ_I(_ultoa_s(v_ul, b, 11, 10), 0);
    CHECK_STR(b, "4294967295");
    CHECK_EQ_I(tc_iph_count - n0, 0);
}
TC_CASE(ui64tow_boundary)
{
    wchar_t w[32]; int n0 = tc_iph_count;
    w[0] = L'z';
    CHECK(_ui64tow_s(UINT64_MAX, w, 20, 10) != 0);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_I(_ui64tow_s(UINT64_MAX, w, 21, 10), 0);
    CHECK_WSTR(w, L"18446744073709551615");
    CHECK_EQ_I(tc_iph_count - n0, 1);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(bad_radix_narrow); TC_RUN(bad_radix_wide);
    TC_RUN(too_small_ultoa); TC_RUN(exact_fit_ultoa); TC_RUN(ui64tow_boundary);
    return tc_end();
}
