/* TC
 * id:    pff_g_roundtrip17
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 g; IEEE 754 exact decimal expansions
 * note:  17 significant digits are identical on legacy and UCRT; exponent normalised by helper.
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

static volatile double a = 0.1, b3 = 0.2, c = 3.0, e = 2.220446049250313080847263336181640625e-16;

TC_CASE(tenth) { char b[64]; sprintf(b, "%.17g", a); CHECK_STR(b, "0.10000000000000001"); }
TC_CASE(sum) { char b[64]; sprintf(b, "%.17g", a + b3); CHECK_STR(b, "0.30000000000000004"); }
TC_CASE(third) { char b[64]; sprintf(b, "%.17g", 1.0 / c); CHECK_STR(b, "0.33333333333333331"); }
TC_CASE(epsilon) { char b[64]; sprintf(b, "%.17g", e); CHECK_STR(norm(b), "2.2204460492503131e-16"); }
TC_CASE(prec15) { char b[64]; sprintf(b, "%.15g", a); CHECK_STR(b, "0.1"); }

int main(void) { tc_begin(); TC_RUN(tenth); TC_RUN(sum); TC_RUN(third); TC_RUN(epsilon); TC_RUN(prec15); return tc_end(); }
