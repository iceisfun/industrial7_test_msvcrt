/* TC
 * id:    cv_neg_strtold_range
 * area:  conv
 * funcs: strtod
 * procs: strtold _strtold_l
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtod, _strtod_l, wcstod, _wcstod_l ("long double is the same as double"; strtold shares strtod's range behaviour); C11 7.22.1.3p8
 * note:  mingw's long double is 80-bit, so strtold is called through a hand-written double-returning prototype (what the MS ABI uses).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef double (__cdecl *strtold_t)(const char *, char **);
static strtold_t p_strtold;
static const char *volatile v_s;
static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(overflow_pos)
{
    char *e = (char *)1; double r, inf = v_one / v_zero; static const char s[] = "1e309";
    if (!p_strtold) TC_SKIP("strtold not exported");
    v_s = s; errno = 0; r = p_strtold(v_s, &e);
    CHECK_BITS_D(r, inf); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 5);
}
TC_CASE(overflow_neg)
{
    char *e = (char *)1; double r, inf = v_one / v_zero; static const char s[] = "-1e309";
    if (!p_strtold) TC_SKIP("strtold not exported");
    v_s = s; errno = 0; r = p_strtold(v_s, &e);
    CHECK_BITS_D(r, -inf); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 6);
}
TC_CASE(underflow)
{
    char *e = (char *)1; double r; static const char s[] = "1e-400";
    if (!p_strtold) TC_SKIP("strtold not exported");
    v_s = s; errno = 0; r = p_strtold(v_s, &e);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 6);
}
TC_CASE(same_as_strtod)
{
    static const char *const in[] = { "1e309", "-1e309", "1e-400", "1.5", "123456789.125", "-0.0", "1e308" };
    size_t i;
    if (!p_strtold) TC_SKIP("strtold not exported");
    for (i = 0; i < sizeof in / sizeof in[0]; i++) {
        char *e1 = 0, *e2 = 0; double a, b;
        v_s = in[i];
        a = p_strtold(v_s, &e1); b = strtod(v_s, &e2);
        CHECK_BITS_D(a, b); CHECK_EQ_P(e1, e2);
    }
}
TC_CASE(no_digits)
{
    char *e = (char *)1; double r; static const char s[] = "zzz";
    if (!p_strtold) TC_SKIP("strtold not exported");
    v_s = s; r = p_strtold(v_s, &e);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_P(e, s);
}

int main(void)
{
    tc_begin();
    p_strtold = (strtold_t)tc_crt_proc("strtold");
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow); TC_RUN(same_as_strtod); TC_RUN(no_digits);
    return tc_end();
}
