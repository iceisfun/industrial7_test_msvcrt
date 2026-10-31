/* TC
 * id:    pff_g_precision_zero
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 g: "if the precision is zero, it is taken as 1"
 * note:  exponent suffix via helper (two digits on ucrt, three on older CRTs).
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
TC_CASE(five)     { char b[64]; v = 5.0; sprintf(b, "%.0g", v); CHECK_STR(b, "5"); }
TC_CASE(half)     { char b[64]; v = 0.5; sprintf(b, "%.0g", v); CHECK_STR(b, "0.5"); }
TC_CASE(exp_style){ char b[64], w[64]; v = 123.0; sprintf(b, "%.0g", v); mk(w, "1", 'e', 0, 2); CHECK_STR(b, w); }
TC_CASE(zero)     { char b[64]; v = 0.0; sprintf(b, "%.0g", v); CHECK_STR(b, "0"); }
TC_CASE(rounds)   { char b[64]; v = 7.25; sprintf(b, "%.0g", v); CHECK_STR(b, "7"); }
TC_CASE(same_as_1){ char a[64], c[64]; v = 987.654; sprintf(a, "%.0g", v); sprintf(c, "%.1g", v); CHECK_STR(a, c); }

int main(void) { tc_begin(); TC_RUN(five); TC_RUN(half); TC_RUN(exp_style); TC_RUN(zero); TC_RUN(rounds); TC_RUN(same_as_1); return tc_end(); }
