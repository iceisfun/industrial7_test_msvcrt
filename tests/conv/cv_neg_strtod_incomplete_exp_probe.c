/* TC
 * id:    cv_neg_strtod_incomplete_exp_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3 (longest valid initial subsequence); MS docs strtod
 * note:  Probes that an exponent marker without digits ("1e", "1e+", "1.5E-") is not consumed: value from the mantissa only,
 *        endptr just after it. Required by C99; legacy (msvcrt/msvcr80-100) scanning of this form is not documented.
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static const char *volatile vs;

TC_CASE(incomplete_exp)
{
    static const char a[] = "1e", b[] = "1e+", c[] = "1.5E-";
    char *e = NULL; double d;
    vs = a; d = tc_p_strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_P(e, a + 1);
    vs = b; d = tc_p_strtod(vs, &e); CHECK_BITS_D(d, 1.0); CHECK_EQ_P(e, b + 1);
    vs = c; d = tc_p_strtod(vs, &e); CHECK_BITS_D(d, 1.5); CHECK_EQ_P(e, c + 3);
}
int main(void) { tc_begin(); TC_LOAD_PROC(strtod); TC_RUN(incomplete_exp); return tc_end(); }
