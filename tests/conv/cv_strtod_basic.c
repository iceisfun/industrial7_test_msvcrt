/* TC
 * id:    cv_strtod_basic
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3; MS docs strtod
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(double, strtod, (const char *, char **));
#define strtod tc_p_strtod

static double D(const char *volatile s, char **e) { return strtod(s, e); }

TC_CASE(plain) { char *e; CHECK_BITS_D(D("3.14", &e), 3.14); CHECK_BITS_D(D("-1e-2", &e), -1e-2); CHECK_BITS_D(D("2e3", &e), 2000.0); }
TC_CASE(leading_trailing_dot)
{
    volatile const char *s = ".5"; char *e = 0;
    CHECK_BITS_D(strtod((const char *)s, &e), 0.5); CHECK_EQ_P(e, s + 2);
    s = "5."; CHECK_BITS_D(strtod((const char *)s, &e), 5.0); CHECK_EQ_P(e, s + 2);
}
TC_CASE(incomplete_exponent)
{
    volatile const char *s = "1e"; char *e = 0;
    CHECK_BITS_D(strtod((const char *)s, &e), 1.0); CHECK_EQ_P(e, s + 1);
    s = "1e+"; e = 0; CHECK_BITS_D(strtod((const char *)s, &e), 1.0); CHECK_EQ_P(e, s + 1);
    s = "1E-x"; e = 0; CHECK_BITS_D(strtod((const char *)s, &e), 1.0); CHECK_EQ_P(e, s + 1);
}
TC_CASE(trailing_garbage)
{
    volatile const char *s = "1.5abc"; char *e = 0;
    CHECK_BITS_D(strtod((const char *)s, &e), 1.5); CHECK_EQ_P(e, s + 3);
}
TC_CASE(no_digits)
{
    volatile const char *s = "abc"; char *e = 0;
    CHECK_BITS_D(strtod((const char *)s, &e), 0.0); CHECK_EQ_P(e, s);
    s = "."; e = 0; CHECK_BITS_D(strtod((const char *)s, &e), 0.0); CHECK_EQ_P(e, s);
    s = "-"; e = 0; CHECK_BITS_D(strtod((const char *)s, &e), 0.0); CHECK_EQ_P(e, s);
}
TC_CASE(whitespace) { volatile const char *s = "  \t2.5"; char *e = 0; CHECK_BITS_D(strtod((const char *)s, &e), 2.5); CHECK_EQ_P(e, s + 6); }
TC_CASE(null_endptr) { CHECK_BITS_D(D("7.25", NULL), 7.25); }

int main(void) { tc_begin(); TC_LOAD_PROC(strtod); CHECK(tc_p_strtod); if (!tc_p_strtod) return 1; TC_RUN(plain); TC_RUN(leading_trailing_dot); TC_RUN(incomplete_exponent); TC_RUN(trailing_garbage); TC_RUN(no_digits); TC_RUN(whitespace); TC_RUN(null_endptr); return tc_end(); }
