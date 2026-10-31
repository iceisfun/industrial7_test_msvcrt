/* TC
 * id:    err_neg_math_edom_more
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: log10 asin acos pow
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs log10, asin, acos return value tables (NaN, errno EDOM); pow (x<0 with non-integer y -> NaN, EDOM); C11 7.12.1
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d1_t)(double);
typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;

#define D1(name, arg) do { d1_t f = (d1_t)tc_crt_proc(#name); double r; if (!f) TC_SKIP(#name " missing"); \
    v_a = (arg); errno = 0; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM); } while (0)

TC_CASE(log10_neg) { D1(log10, -1.0); }
TC_CASE(asin_2) { D1(asin, 2.0); }
TC_CASE(acos_m2) { D1(acos, -2.0); }
TC_CASE(pow_neg_frac)
{
    d2_t f = (d2_t)tc_crt_proc("pow"); double r; if (!f) TC_SKIP("pow missing");
    v_a = -8.0; v_b = 0.5; errno = 0; r = f(v_a, v_b); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM);
}
TC_CASE(errno_overwritten_to_edom)
{
    d1_t f = (d1_t)tc_crt_proc("log10"); double r; if (!f) TC_SKIP("log10 missing");
    v_a = -1.0; errno = ERANGE; r = f(v_a); CHECK_IS_NAN(r); CHECK_EQ_I(errno, EDOM);
}
int main(void)
{
    tc_begin();
    TC_RUN(log10_neg); TC_RUN(asin_2); TC_RUN(acos_m2); TC_RUN(pow_neg_frac); TC_RUN(errno_overwritten_to_edom);
    errno = 0;
    return tc_end();
}
