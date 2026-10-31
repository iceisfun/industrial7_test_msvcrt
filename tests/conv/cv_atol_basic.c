/* TC
 * id:    cv_atol_basic
 * area:  conv
 * funcs: atol
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.2; MS docs atol
 */
#include "tc.h"
#include <stdlib.h>

static long A(const char *volatile s) { return atol(s); }

TC_CASE(decimal) { CHECK_EQ_I(A("123456"), 123456L); CHECK_EQ_I(A("+9"), 9L); }
TC_CASE(negative) { CHECK_EQ_I(A("-987654"), -987654L); }
TC_CASE(whitespace) { CHECK_EQ_I(A(" \t\n\v\f\r42"), 42L); CHECK_EQ_I(A("  -42"), -42L); }
TC_CASE(garbage_tail) { CHECK_EQ_I(A("77xyz"), 77L); CHECK_EQ_I(A("5.9e3"), 5L); }
TC_CASE(long_is_32bit) { CHECK_EQ_I(sizeof(long), 4); CHECK_EQ_I(A("2147483647"), 2147483647L); }
TC_CASE(long_min) { CHECK_EQ_I(A("-2147483648"), -2147483647L - 1); }
TC_CASE(no_digits) { CHECK_EQ_I(A("zzz"), 0L); CHECK_EQ_I(A(""), 0L); CHECK_EQ_I(A("-"), 0L); }
TC_CASE(hex_not_parsed) { CHECK_EQ_I(A("0x10"), 0L); }

int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(negative); TC_RUN(whitespace); TC_RUN(garbage_tail); TC_RUN(long_is_32bit); TC_RUN(long_min); TC_RUN(no_digits); TC_RUN(hex_not_parsed); return tc_end(); }
