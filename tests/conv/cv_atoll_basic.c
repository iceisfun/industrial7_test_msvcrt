/* TC
 * id:    cv_atoll_basic
 * area:  conv
 * funcs: atoll
 * crt:   >=120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs atoll, _atoi64 (atoll in msvcr120 and ucrtbase)
 */
#include "tc.h"
#include <stdlib.h>

static long long A(const char *volatile s) { return atoll(s); }

TC_CASE(max) { CHECK_EQ_I(A("9223372036854775807"), 0x7fffffffffffffffLL); }
TC_CASE(min) { CHECK_EQ_I(A("-9223372036854775808"), -0x7fffffffffffffffLL - 1); }
TC_CASE(whitespace) { CHECK_EQ_I(A(" \t\n\v\f\r  123"), 123LL); CHECK_EQ_I(A("\t-123"), -123LL); }
TC_CASE(garbage_tail) { CHECK_EQ_I(A("5000000000abc"), 5000000000LL); CHECK_EQ_I(A("12.5"), 12LL); }
TC_CASE(no_digits) { CHECK_EQ_I(A("x1"), 0LL); CHECK_EQ_I(A(""), 0LL); CHECK_EQ_I(A("+"), 0LL); }
TC_CASE(beyond_32bit) { CHECK_EQ_I(A("4294967296"), 4294967296LL); CHECK_EQ_I(A("-3000000000"), -3000000000LL); CHECK_EQ_I(A("+8589934592"), 8589934592LL); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(min); TC_RUN(whitespace); TC_RUN(garbage_tail); TC_RUN(no_digits); TC_RUN(beyond_32bit); return tc_end(); }
