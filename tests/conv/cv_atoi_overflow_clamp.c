/* TC
 * id:    cv_atoi_overflow_clamp
 * area:  conv
 * funcs: atoi atol _atoi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atoi, _atoi64 return value: INT_MAX/INT_MIN, LONG_MAX/LONG_MIN, _I64_MAX/_I64_MIN on overflow
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static int  I(const char *volatile s) { return atoi(s); }
static long L(const char *volatile s) { return atol(s); }
static __int64 Q(const char *volatile s) { return _atoi64(s); }

TC_CASE(atoi_max) { CHECK_EQ_I(I("2147483648"), INT_MAX); CHECK_EQ_I(I("2147483647"), INT_MAX); CHECK_EQ_I(I("4294967296"), INT_MAX); }
TC_CASE(atoi_min) { CHECK_EQ_I(I("-2147483649"), INT_MIN); CHECK_EQ_I(I("-2147483648"), INT_MIN); }
TC_CASE(atol)  { CHECK_EQ_I(L("2147483648"), LONG_MAX); CHECK_EQ_I(L("-2147483649"), LONG_MIN); }
TC_CASE(i64_max) { CHECK_EQ_I(Q("9223372036854775808"), _I64_MAX); }
TC_CASE(i64_min) { CHECK_EQ_I(Q("-9223372036854775809"), _I64_MIN); }
TC_CASE(long_digits)
{
    static const char big[] = "99999999999999999999999999999999999999999999999999999999999999999999";
    static const char nbig[] = "-99999999999999999999999999999999999999999999999999999999999999999999";
    CHECK_EQ_I(I(big), INT_MAX); CHECK_EQ_I(I(nbig), INT_MIN);
    CHECK_EQ_I(Q(big), _I64_MAX); CHECK_EQ_I(Q(nbig), _I64_MIN);
}

int main(void) { tc_begin(); TC_RUN(atoi_max); TC_RUN(atoi_min); TC_RUN(atol); TC_RUN(i64_max); TC_RUN(i64_min); TC_RUN(long_digits); return tc_end(); }
