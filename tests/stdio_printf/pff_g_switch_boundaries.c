/* TC
 * id:    pff_g_switch_boundaries
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 g (use e style if X < -4 or X >= P)
 * note:  Exponent digit count differs between legacy and UCRT; normalised by helper.
 */
#include "tc.h"
#include <stdio.h>

static char *norm(char *s)  /* reduce a 3-digit zero-padded exponent (legacy CRTs) to 2 digits */
{
    char *e = s;
    while (*e && *e != 'e' && *e != 'E' && *e != 'p') e++;
    if (*e && (e[1] == '+' || e[1] == '-') && e[2] == '0' && e[3] >= '0' && e[3] <= '9' && e[4] >= '0' && e[4] <= '9' && !e[5]) {
        e[2] = e[3]; e[3] = e[4]; e[4] = 0;
    }
    return s;
}

static volatile double a = 123456.0, b7 = 1234567.0, c = 0.0001, d = 0.000099999, e1 = 999.4, e2 = 999.6;

TC_CASE(p_boundary_fixed) { char b[64]; sprintf(b, "%g", a); CHECK_STR(b, "123456"); }
TC_CASE(p_boundary_exp) { char b[64]; sprintf(b, "%g", b7); CHECK_STR(norm(b), "1.23457e+06"); }
TC_CASE(minus4_fixed) { char b[64]; sprintf(b, "%g", c); CHECK_STR(b, "0.0001"); }
TC_CASE(minus5_exp) { char b[64]; sprintf(b, "%g", d); CHECK_STR(norm(b), "9.9999e-05"); }
TC_CASE(round_stays_fixed) { char b[64]; sprintf(b, "%.3g", e1); CHECK_STR(b, "999"); }
TC_CASE(round_bumps_exp) { char b[64]; sprintf(b, "%.3g", e2); CHECK_STR(norm(b), "1e+03"); }

int main(void) { tc_begin(); TC_RUN(p_boundary_fixed); TC_RUN(p_boundary_exp); TC_RUN(minus4_fixed); TC_RUN(minus5_exp); TC_RUN(round_stays_fixed); TC_RUN(round_bumps_exp); return tc_end(); }
