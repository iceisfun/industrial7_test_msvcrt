/* TC
 * id:    pff_g_uppercase
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (G, E)
 * note:  Exponent digit count normalised by helper.
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

static volatile double a = 1e-10, b15 = 1.5, c = 100000.0, d = 1e6;

TC_CASE(G_small) { char b[64]; sprintf(b, "%G", a); CHECK_STR(norm(b), "1E-10"); }
TC_CASE(E_basic) { char b[64]; sprintf(b, "%E", b15); CHECK_STR(norm(b), "1.500000E+00"); }
TC_CASE(G_fixed) { char b[64]; sprintf(b, "%G", c); CHECK_STR(b, "100000"); }
TC_CASE(G_1e6) { char b[64]; sprintf(b, "%G", d); CHECK_STR(norm(b), "1E+06"); }

int main(void) { tc_begin(); TC_RUN(G_small); TC_RUN(E_basic); TC_RUN(G_fixed); TC_RUN(G_1e6); return tc_end(); }
