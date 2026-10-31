/* TC
 * id:    cv_atoi_limits_wchar_variants
 * area:  conv
 * funcs: _wtoi _wtoi64 atoi
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.2; MS docs atoi, _atoi64, _wtoi, _wtoi64
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static int N(const char *volatile s) { return atoi(s); }
static int W(const wchar_t *volatile s) { return _wtoi(s); }
static __int64 W64(const wchar_t *volatile s) { return _wtoi64(s); }

TC_CASE(int_max) { CHECK_EQ_I(N("2147483647"), INT_MAX); CHECK_EQ_I(W(L"2147483647"), INT_MAX); }
TC_CASE(int_min) { CHECK_EQ_I(N("-2147483648"), INT_MIN); CHECK_EQ_I(W(L"-2147483648"), INT_MIN); }
TC_CASE(wtoi64_limits)
{
    CHECK_EQ_I(W64(L"9223372036854775807"), _I64_MAX);
    CHECK_EQ_I(W64(L"-9223372036854775808"), _I64_MIN);
    CHECK_EQ_I(W64(L"4294967296"), 4294967296LL);
}
TC_CASE(leading_ws)
{
    CHECK_EQ_I(N(" \t\n 42"), 42); CHECK_EQ_I(W(L" \t\n 42"), 42);
    CHECK_EQ_I(N("  -17x"), -17); CHECK_EQ_I(W(L"  -17x"), -17);
    CHECK_EQ_I(W64(L"\r\v\f 123456789012"), 123456789012LL);
}
TC_CASE(wide_stops) { CHECK_EQ_I(W(L"12abc"), 12); CHECK_EQ_I(W(L"+8"), 8); CHECK_EQ_I(W(L"abc"), 0); CHECK_EQ_I(W(L""), 0); }

int main(void) { tc_begin(); TC_RUN(int_max); TC_RUN(int_min); TC_RUN(wtoi64_limits); TC_RUN(leading_ws); TC_RUN(wide_stops); return tc_end(); }
