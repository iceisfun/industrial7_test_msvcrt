/* TC
 * id:    gcv_neg_atoll_l_overflow
 * area:  conv
 * funcs: _atoll_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi, _atoi_l: LLONG_MAX/LLONG_MIN on overflow
 * note:  Overflow saturation asserted on ucrtbase only; legacy CRTs are covered by gcv_probe_conv_overflow_legacy. errno ERANGE not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t L;
static long long A(const char *volatile s) { return _atoll_l(s, L); }

TC_CASE(pos_overflow) { CHECK_EQ_I(A("9223372036854775808"), 0x7fffffffffffffffLL); CHECK_EQ_I(A("99999999999999999999999"), 0x7fffffffffffffffLL); }
TC_CASE(neg_overflow) { CHECK_EQ_I(A("-9223372036854775809"), -0x7fffffffffffffffLL - 1); CHECK_EQ_I(A("-99999999999999999999999"), -0x7fffffffffffffffLL - 1); }
TC_CASE(non_numeric)
{
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(A("abc"), 0LL); CHECK_EQ_I(A(""), 0LL); CHECK_EQ_I(A("-"), 0LL); CHECK_EQ_I(A("+"), 0LL);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(pos_overflow); TC_RUN(neg_overflow); TC_RUN(non_numeric);
    _free_locale(L);
    return tc_end();
}
