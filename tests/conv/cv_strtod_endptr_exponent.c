/* TC
 * id:    cv_strtod_endptr_exponent
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3 (longest initial subsequence of expected form; endptr = nptr if none)
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
static double P(const char *s, long *off) { char *e = NULL; double d; vs = s; d = strtod(vs, &e); *off = (long)(e - s); return d; }

TC_CASE(trailing_junk) { long o; CHECK_BITS_D(P("1e5x", &o), 100000.0); CHECK_EQ_I(o, 3); }
TC_CASE(plus_exp) { long o; CHECK_BITS_D(P("1e+5", &o), 100000.0); CHECK_EQ_I(o, 4); }
TC_CASE(dot_then_exp) { long o; CHECK_BITS_D(P("1.e2", &o), 100.0); CHECK_EQ_I(o, 4); }
TC_CASE(double_dot) { long o; CHECK_BITS_D(P("..5", &o), 0.0); CHECK_EQ_I(o, 0); }
TC_CASE(comma_stops) { long o; CHECK_BITS_D(P("1,5", &o), 1.0); CHECK_EQ_I(o, 1); }
TC_CASE(second_dot) { long o; CHECK_BITS_D(P("1.5.5", &o), 1.5); CHECK_EQ_I(o, 3); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); 
    TC_RUN(trailing_junk); TC_RUN(plus_exp);  TC_RUN(dot_then_exp);
    TC_RUN(double_dot); TC_RUN(comma_stops); TC_RUN(second_dot);
    return tc_end();
}
