/* TC
 * id:    cv_strtod_incomplete_exp_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * note:  Probes that "1e", "1e-" parse as 1.0 with endptr just after the mantissa on every CRT (standard; old CRT endptr handling of
 *        an incomplete exponent unverified).
 * ref:   C11 7.22.1.3 (longest initial subsequence of expected form; endptr = nptr if none)
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
static double P(const char *s, long *off) { char *e = NULL; double d; vs = s; d = strtod(vs, &e); *off = (long)(e - s); return d; }

TC_CASE(incomplete_exp) { long o; CHECK_BITS_D(P("1e-", &o), 1.0); CHECK_EQ_I(o, 1); CHECK_BITS_D(P("1e", &o), 1.0); CHECK_EQ_I(o, 1); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); 
      TC_RUN(incomplete_exp); 
      
    return tc_end();
}
