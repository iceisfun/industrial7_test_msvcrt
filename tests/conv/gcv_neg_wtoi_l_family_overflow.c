/* TC
 * id:    gcv_neg_wtoi_l_family_overflow
 * area:  conv
 * funcs: _wtoi_l _wtol_l _wtoi64_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l, _wtol_l, _wtoi64_l: INT_MAX/INT_MIN etc on overflow
 * note:  Saturation asserted on ucrtbase only; legacy behaviour in gcv_probe_conv_overflow_legacy.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <limits.h>

static _locale_t L;
static int  I(const wchar_t *volatile s) { return _wtoi_l(s, L); }
static long Lg(const wchar_t *volatile s) { return _wtol_l(s, L); }
static __int64 Q(const wchar_t *volatile s) { return _wtoi64_l(s, L); }

TC_CASE(i64) { CHECK_EQ_I(Q(L"9223372036854775808"), 0x7fffffffffffffffLL); CHECK_EQ_I(Q(L"-9223372036854775809"), -0x7fffffffffffffffLL - 1); }
TC_CASE(lng) { CHECK_EQ_I(Lg(L"4294967296"), LONG_MAX); CHECK_EQ_I(Lg(L"-4294967297"), LONG_MIN); }
TC_CASE(int_) { CHECK_EQ_I(I(L"2147483648"), INT_MAX); CHECK_EQ_I(I(L"-2147483649"), INT_MIN); }
TC_CASE(no_digits)
{
    tc_iph_count = 0;
    CHECK_EQ_I(I(L""), 0); CHECK_EQ_I(I(L"-"), 0); CHECK_EQ_I(I(L"abc"), 0);
    CHECK_EQ_I(Lg(L""), 0); CHECK_EQ_I(Lg(L"abc"), 0);
    CHECK_EQ_I(Q(L"-"), 0); CHECK_EQ_I(Q(L"abc"), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(i64); TC_RUN(lng); TC_RUN(int_); TC_RUN(no_digits);
    _free_locale(L);
    return tc_end();
}
