/* TC
 * id:    pff_e_flags_width
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 flags and field width; MS docs printf
 * note:  exponent suffix by helper (two digits on ucrt, three on older CRTs).
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
static int len_(const char *s) { int n = 0; while (s[n]) n++; return n; }
/* mode 0: right-align with spaces, 1: left-justify, 2: zero pad after the sign */
static void pad(char *o, const char *s, int w, int mode)
{
    int n = len_(s), k = w > n ? w - n : 0, i;
    if (mode == 0) { for (i = 0; i < k; i++) *o++ = (char)32; while (*s) *o++ = *s++; }
    else if (mode == 1) { while (*s) *o++ = *s++; for (i = 0; i < k; i++) *o++ = (char)32; }
    else { if (*s == 45 || *s == 43 || *s == 32) *o++ = *s++; for (i = 0; i < k; i++) *o++ = (char)48; while (*s) *o++ = *s++; }
    *o = 0;
}
static void close_(char *w) { int n = len_(w); w[n] = (char)93; w[n+1] = 0; }
static void bar_(char *w) { int n = len_(w); w[n] = (char)124; w[n+1] = 0; }
static volatile double v;
TC_CASE(plus)      { char b[64], w[64]; v = 1.5; sprintf(b, "[%+e]", v); mk(w, "[+1.500000", 'e', 0, 0); close_(w); CHECK_STR(b, w); }
TC_CASE(space)     { char b[64], w[64]; v = 1.5; sprintf(b, "[% e]", v); mk(w, "[ 1.500000", 'e', 0, 0); close_(w); CHECK_STR(b, w); }
TC_CASE(zero_pad)  { char b[64], w[64], t[64]; v = 1.5; sprintf(b, "%015.3e", v); mk(t, "1.500", 'e', 0, 0); pad(w, t, 15, 2); CHECK_STR(b, w); }
TC_CASE(zero_pad_neg) { char b[64], w[64], t[64]; v = -1.5; sprintf(b, "%015.3e", v); mk(t, "-1.500", 'e', 0, 0); pad(w, t, 15, 2); CHECK_STR(b, w); }
TC_CASE(left)      { char b[64], w[64], t[64]; v = 1.5; sprintf(b, "%-15.3e|", v); mk(t, "1.500", 'e', 0, 0); pad(w, t, 15, 1); bar_(w); CHECK_STR(b, w); }
TC_CASE(right_upper) { char b[64], w[64], t[64]; v = 1.5; sprintf(b, "%15.3E", v); mk(t, "1.500", 'E', 0, 0); pad(w, t, 15, 0); CHECK_STR(b, w); }
TC_CASE(minus_beats_zero) { char b[64], w[64], t[64]; v = 1.5; sprintf(b, "%-015.3e|", v); mk(t, "1.500", 'e', 0, 0); pad(w, t, 15, 1); bar_(w); CHECK_STR(b, w); }

int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(space); TC_RUN(zero_pad); TC_RUN(zero_pad_neg); TC_RUN(left); TC_RUN(right_upper); TC_RUN(minus_beats_zero); return tc_end(); }
