/* TC
 * id:    cv_neg_atoi_overflow_errno
 * area:  conv
 * funcs: atoi atol _atoi64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi: out-of-range sets errno to ERANGE
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

TC_CASE(atoi_erange) { volatile const char *s = "99999999999"; int r; errno = 0; r = atoi((const char *)s); CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(atoi_neg_erange) { volatile const char *s = "-99999999999"; int r; errno = 0; r = atoi((const char *)s); CHECK_EQ_I(r, INT_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(atol_erange) { volatile const char *s = "-99999999999999"; long r; errno = 0; r = atol((const char *)s); CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(i64_erange)
{
    volatile const char *s = "9223372036854775808"; volatile const char *n = "-9223372036854775809"; __int64 r;
    errno = 0; r = _atoi64((const char *)s); CHECK_EQ_I(r, _I64_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0; r = _atoi64((const char *)n); CHECK_EQ_I(r, _I64_MIN); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(in_range_untouched)
{
    volatile const char *s = "2147483647"; volatile const char *t = "-2147483648"; volatile const char *u = "9223372036854775807";
    errno = 0; CHECK_EQ_I(atoi((const char *)s), INT_MAX); CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(atoi((const char *)t), INT_MIN); CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(_atoi64((const char *)u), _I64_MAX); CHECK_EQ_I(errno, 0);
}

int main(void) { tc_begin(); TC_RUN(atoi_erange); TC_RUN(atoi_neg_erange); TC_RUN(atol_erange); TC_RUN(i64_erange); TC_RUN(in_range_untouched); return tc_end(); }
