/* TC
 * id:    cv_wtoi_basic
 * area:  conv
 * funcs: _wtoi _wtol _wtoi64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atoi, _atoi_l, _wtoi, _atoi64, _wtoi64: leading whitespace skipped, optional sign, stops at first non-digit, 0 if no conversion
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static int I(const wchar_t *volatile s) { return _wtoi(s); }
static long L(const wchar_t *volatile s) { return _wtol(s); }
static __int64 Q(const wchar_t *volatile s) { return _wtoi64(s); }

TC_CASE(simple) { CHECK_EQ_I(I(L"42"), 42); CHECK_EQ_I(I(L"-17"), -17); CHECK_EQ_I(I(L"+5"), 5); CHECK_EQ_I(I(L"0"), 0); }
TC_CASE(ws) { CHECK_EQ_I(I(L"  \t\n 42"), 42); CHECK_EQ_I(I(L" \r-8"), -8); }
TC_CASE(tail) { CHECK_EQ_I(I(L"123abc"), 123); CHECK_EQ_I(I(L"12 34"), 12); CHECK_EQ_I(I(L"7.9"), 7); }
TC_CASE(no_digits) { CHECK_EQ_I(I(L"abc"), 0); CHECK_EQ_I(I(L""), 0); CHECK_EQ_I(I(L"-"), 0); CHECK_EQ_I(I(L"   "), 0); }
TC_CASE(i64_big)
{
    CHECK_EQ_I(Q(L"9223372036854775807"), 9223372036854775807LL);
    CHECK_EQ_I(Q(L"-9223372036854775807"), -9223372036854775807LL);
    CHECK_EQ_I(Q(L"4294967296"), 4294967296LL);
    CHECK_EQ_I(Q(L"  -12abc"), -12);
}
TC_CASE(wtol) { CHECK_EQ_I(L(L"-99"), -99); CHECK_EQ_I(L(L"2147483647"), 2147483647L); CHECK_EQ_I(L(L"x1"), 0); }

int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(ws); TC_RUN(tail); TC_RUN(no_digits); TC_RUN(i64_big); TC_RUN(wtol); return tc_end(); }
