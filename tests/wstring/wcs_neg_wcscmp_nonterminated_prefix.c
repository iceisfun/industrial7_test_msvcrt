/* TC
 * id:    wcs_neg_wcscmp_nonterminated_prefix
 * area:  wstring
 * funcs: wcscmp wcsncmp _wcsicmp wcsstr wcsspn wcscspn
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.29.4.4.1, 7.29.4.4.4, 7.29.4.5.7, 7.29.4.5.3, 7.29.4.5.4; MS docs _wcsicmp
 * note:  Cases that must NOT match / must NOT compare equal.
 */
#include "tc.h"
#include <wchar.h>

static const wchar_t *volatile vab = L"ab", *volatile vabc = L"abc", *volatile vhay = L"hello", *volatile vlong = L"hello!",
    *volatile vlast = L"hellp", *volatile vempty = L"", *volatile vx = L"xyz";

TC_CASE(needle_longer_than_hay_null) { CHECK(wcsstr(vhay, vlong) == NULL); }
TC_CASE(needle_last_unit_differs_null) { CHECK(wcsstr(vhay, vlast) == NULL); }
TC_CASE(wcsspn_empty_set_0) { CHECK_EQ_U(wcsspn(vhay, vempty), 0); }
TC_CASE(wcscspn_empty_set_len) { CHECK_EQ_U(wcscspn(vhay, vempty), 5); CHECK_EQ_U(wcscspn(vempty, vx), 0); }
TC_CASE(prefix_less) { CHECK(wcscmp(vab, vabc) < 0); CHECK(wcscmp(vabc, vab) > 0); CHECK(wcscmp(vempty, vab) < 0); }
TC_CASE(ncmp_ignores_after_n)
{
    CHECK_EQ_I(wcsncmp(vab, vabc, 2), 0);
    CHECK(wcsncmp(vab, vabc, 3) < 0);
    CHECK_EQ_I(wcsncmp(vabc, vx, 0), 0);
}
TC_CASE(icmp_prefix_not_equal) { CHECK(_wcsicmp(vab, vabc) < 0); CHECK(_wcsicmp(vabc, vab) > 0); CHECK(_wcsicmp(vempty, vab) < 0); }
int main(void)
{
    tc_begin(); TC_RUN(needle_longer_than_hay_null); TC_RUN(needle_last_unit_differs_null); TC_RUN(wcsspn_empty_set_0);
    TC_RUN(wcscspn_empty_set_len); TC_RUN(prefix_less); TC_RUN(ncmp_ignores_after_n); TC_RUN(icmp_prefix_not_equal);
    return tc_end();
}
