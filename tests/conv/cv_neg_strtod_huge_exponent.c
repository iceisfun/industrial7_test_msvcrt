/* TC
 * id:    cv_neg_strtod_huge_exponent
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3p10: overflow -> +-HUGE_VAL and ERANGE; underflow -> ERANGE (implementation defined whether result is 0)
 */
#include "tc.h"
#include <errno.h>
#include <math.h>

TC_DECL_PROC(double, strtod, (const char *, char **));

TC_CASE(exponent_digits_overflow_int)
{
    char *e; double r;
    static const char s[] = "1e99999999999999999999";
    errno = 0; r = tc_p_strtod(s, &e);
    CHECK(r == HUGE_VAL); CHECK_EQ_I(errno, 34); CHECK(e == s + sizeof s - 1);
}
TC_CASE(negative_overflow)
{
    char *e; double r;
    static const char s[] = "-1e99999999999999999999";
    errno = 0; r = tc_p_strtod(s, &e);
    CHECK(r == -HUGE_VAL); CHECK_EQ_I(errno, 34); CHECK(e == s + sizeof s - 1);
}
TC_CASE(neg_exponent_underflow)
{
    char *e; double r;
    static const char s[] = "1e-99999999999999999999";
    errno = 0; r = tc_p_strtod(s, &e);
    CHECK_BITS_D(r, 0.0); CHECK(e == s + sizeof s - 1);
}
TC_CASE(zero_mantissa_huge_exponent)
{
    char *e; double r;
    static const char s[] = "0e99999999999999999999";
    r = tc_p_strtod(s, &e);
    CHECK_BITS_D(r, 0.0); CHECK(e == s + sizeof s - 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(exponent_digits_overflow_int); TC_RUN(negative_overflow); TC_RUN(neg_exponent_underflow); TC_RUN(zero_mantissa_huge_exponent);
    return tc_end();
}
