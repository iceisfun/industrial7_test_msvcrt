/* TC
 * id:    gcv_atoll_l_basic
 * area:  conv
 * funcs: _atoll_l _create_locale _free_locale
 * crt:   >=120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atoi, _atoi_l, _wtoi, _wtoi_l
 * note:  _atoll_l treated as exported from msvcr120 on (not imported from msvcr110/100 per build check). Locale is an explicit C locale handle.
 *        TEST BUG FIXED (class A): the call was compiled against an implicit int declaration (mingw-w64 has no prototype for _atoll_l), so Windows'
 *        64-bit result was truncated to int. With a correct prototype ucrtbase on Win10 22H2 returns the full 64-bit saturated/exact value
 *        (verified with a GetProcAddress probe: atoll, _atoll_l, _atoi64(_l), _wtoll(_l), _wtoi64(_l), strtoll and wcstoll all return LLONG_MAX / LLONG_MIN on overflow
 *        and 0x12a05f200 for 5000000000). Assertions are unchanged.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stddef.h>

/* mingw-w64 headers do not declare _atoll_l: an implicit declaration returns int, truncating the 64-bit result to 32 bits
 * and sign-extending it (observed: "9223372036854775807" -> -1). Real ucrtbase returns the full long long, so declare it. */
extern long long __cdecl _atoll_l(const char *, _locale_t);

static _locale_t L;
static long long A(const char *volatile s) { return _atoll_l(s, L); }

TC_CASE(decimal) { CHECK_EQ_I(A("123"), 123LL); CHECK_EQ_I(A("  \t\n-45"), -45LL); CHECK_EQ_I(A("+67"), 67LL); }
TC_CASE(trailing) { CHECK_EQ_I(A("12abc"), 12LL); CHECK_EQ_I(A("-9 9"), -9LL); CHECK_EQ_I(A("7.9"), 7LL); }
TC_CASE(limits) { CHECK_EQ_I(A("9223372036854775807"), 0x7fffffffffffffffLL); CHECK_EQ_I(A("-9223372036854775808"), -0x7fffffffffffffffLL - 1); }
TC_CASE(no_digits) { CHECK_EQ_I(A("abc"), 0LL); CHECK_EQ_I(A(""), 0LL); }
TC_CASE(zeros) { CHECK_EQ_I(A("0"), 0LL); CHECK_EQ_I(A("-0"), 0LL); CHECK_EQ_I(A("000042"), 42LL); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    if (!L) return tc_end() | 1;
    TC_RUN(decimal); TC_RUN(trailing); TC_RUN(limits); TC_RUN(no_digits); TC_RUN(zeros);
    _free_locale(L);
    return tc_end();
}
