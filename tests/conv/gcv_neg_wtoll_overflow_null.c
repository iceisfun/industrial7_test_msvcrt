/* TC
 * id:    gcv_neg_wtoll_overflow_null
 * area:  conv
 * funcs: _wtoll _wtoll_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l (NULL: invalid parameter, EINVAL, 0; overflow saturates)
 * note:  Overflow asserted on ucrtbase only; msvcr120 is covered by gcv_probe_conv_overflow_legacy. NULL checks are in gcv_neg_wtoll_null_iph.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t L;
static long long W(const wchar_t *volatile s) { return _wtoll(s); }
static long long WL(const wchar_t *volatile s) { return _wtoll_l(s, L); }

TC_CASE(overflow)
{
    CHECK_EQ_I(W(L"9223372036854775808"), 0x7fffffffffffffffLL);
    CHECK_EQ_I(W(L"-9223372036854775809"), -0x7fffffffffffffffLL - 1);
    CHECK_EQ_I(WL(L"9223372036854775808"), 0x7fffffffffffffffLL);
    CHECK_EQ_I(WL(L"-9223372036854775809"), -0x7fffffffffffffffLL - 1);
}
TC_CASE(long_digits)
{
    CHECK_EQ_I(W(L"99999999999999999999999999999999"), 0x7fffffffffffffffLL);
    CHECK_EQ_I(W(L"-99999999999999999999999999999999"), -0x7fffffffffffffffLL - 1);
}
TC_CASE(non_numeric)
{
    tc_iph_count = 0;
    CHECK_EQ_I(W(L"xyz"), 0LL); CHECK_EQ_I(WL(L"xyz"), 0LL); CHECK_EQ_I(W(L"+"), 0LL);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(overflow); TC_RUN(long_digits); TC_RUN(non_numeric);
    _free_locale(L);
    return tc_end();
}
