/* TC
 * id:    cv_atoi_basic
 * area:  conv
 * funcs: atoi
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.2; MS docs atoi
 */
#include "tc.h"
#include <stdlib.h>

static int A(const char *volatile s) { return atoi(s); }

TC_CASE(decimal) { CHECK_EQ_I(A("123"), 123); CHECK_EQ_I(A("0"), 0); CHECK_EQ_I(A("2147483647"), 2147483647); }
TC_CASE(negative) { CHECK_EQ_I(A("-45"), -45); CHECK_EQ_I(A("-2147483647"), -2147483647); }
TC_CASE(plus) { CHECK_EQ_I(A("+7"), 7); }
TC_CASE(whitespace)
{
    CHECK_EQ_I(A("  12"), 12); CHECK_EQ_I(A("\t13"), 13); CHECK_EQ_I(A("\n14"), 14);
    CHECK_EQ_I(A("\v15"), 15); CHECK_EQ_I(A("\f16"), 16); CHECK_EQ_I(A("\r17"), 17);
    CHECK_EQ_I(A(" \t\n-18"), -18);
}
TC_CASE(stops) { CHECK_EQ_I(A("12abc"), 12); CHECK_EQ_I(A("12.9"), 12); CHECK_EQ_I(A("1 2"), 1); CHECK_EQ_I(A("-3x"), -3); }
TC_CASE(no_digits) { CHECK_EQ_I(A("abc"), 0); CHECK_EQ_I(A("-"), 0); CHECK_EQ_I(A("+"), 0); CHECK_EQ_I(A("- 5"), 0); CHECK_EQ_I(A("0x10"), 0); }
TC_CASE(empty) { CHECK_EQ_I(A(""), 0); CHECK_EQ_I(A("   "), 0); }
TC_CASE(neg_zero) { CHECK_EQ_I(A("-0"), 0); CHECK_EQ_I(A("+0"), 0); }

int main(void) { tc_begin(); TC_RUN(decimal); TC_RUN(negative); TC_RUN(plus); TC_RUN(whitespace); TC_RUN(stops); TC_RUN(no_digits); TC_RUN(empty); TC_RUN(neg_zero); return tc_end(); }
