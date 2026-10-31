/* TC
 * id:    pff_e_carry_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 e/E, # flag; MS docs printf
 * note:  exponent suffix built with a helper (two digits on ucrt, three on older CRTs).
 */
#include "tc.h"
#include <stdio.h>

static void mk(char *o, const char *m, char ech, int neg, int e)
{
    int nd = (TC_BELOW(140) || e >= 100) ? 3 : 2, i;
    char t[4];
    while (*m) *o++ = *m++;
    *o++ = ech; *o++ = neg ? (char)45 : (char)43;
    t[0] = (char)(48 + (e / 100) % 10); t[1] = (char)(48 + (e / 10) % 10); t[2] = (char)(48 + e % 10);
    for (i = 3 - nd; i < 3; i++) *o++ = t[i];
    *o = 0;
}
static volatile double v;
TC_CASE(carry_to_next_exp) { char b[64], w[64]; v = 9.99; sprintf(b, "%.1e", v); mk(w, "1.0", 'e', 0, 1); CHECK_STR(b, w); }
TC_CASE(tie_up)            { char b[64], w[64]; v = 9.5; sprintf(b, "%.0e", v); mk(w, "1", 'e', 0, 1); CHECK_STR(b, w); }
TC_CASE(alt_point)         { char b[64], w[64]; v = 1.0; sprintf(b, "%#.0e", v); mk(w, "1.", 'e', 0, 0); CHECK_STR(b, w); }
TC_CASE(no_alt_no_point)   { char b[64], w[64]; v = 1.0; sprintf(b, "%.0e", v); mk(w, "1", 'e', 0, 0); CHECK_STR(b, w); }
TC_CASE(trailing_zeros)    { char b[64], w[64]; v = 1.0; sprintf(b, "%.10e", v); mk(w, "1.0000000000", 'e', 0, 0); CHECK_STR(b, w); }
TC_CASE(carry_prec0)       { char b[64], w[64]; v = 99999.5; sprintf(b, "%.0e", v); mk(w, "1", 'e', 0, 5); CHECK_STR(b, w); }
TC_CASE(carry_negative)    { char b[64], w[64]; v = -9.99; sprintf(b, "%.1e", v); mk(w, "-1.0", 'e', 0, 1); CHECK_STR(b, w); }
TC_CASE(small_exp_neg)     { char b[64], w[64]; v = 9.999e-10; sprintf(b, "%.2e", v); mk(w, "1.00", 'e', 1, 9); CHECK_STR(b, w); }

int main(void) { tc_begin(); TC_RUN(carry_to_next_exp); TC_RUN(tie_up); TC_RUN(alt_point); TC_RUN(no_alt_no_point); TC_RUN(trailing_zeros); TC_RUN(carry_prec0); TC_RUN(carry_negative); TC_RUN(small_exp_neg); return tc_end(); }
