/* TC
 * id:    cv_strtod_longdigits_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.22.1.3 (result is correctly rounded when the input has more digits than DECIMAL_DIG)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): ucrtbase strtod keeps only the first ~768 significant decimal digits (sticky information beyond that
 *        is dropped, it is NOT exact for arbitrarily long input as the original comment assumed). A tie-breaking digit at fractional digit 750 (total
 *        766 sig digits) rounds up; at digits 760/768/769/780/800 (>768 sig digits) the tie goes to even. C11 (5.2.4.2.1, 7.22.1.3) requires correct rounding
 *        only up to DECIMAL_DIG digits, so this is conforming, not a deviation. 800-digit mantissa of 0.1 and the exact-tie case are unchanged. Case renamed.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static char buf[2048];
static double FB(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(tenth_800_digits)
{   /* "0." + 799 zeros + "1" would be 1e-800; instead 0.1 followed by 800 zeros */
    int i, n = 0; buf[n++] = '0'; buf[n++] = '.'; buf[n++] = '1';
    for (i = 0; i < 800; i++) buf[n++] = '0';
    buf[n] = 0;
    CHECK_BITS_D(strtod(buf, NULL), FB(0x3FB999999999999AULL));
}
static void tie_plus_one_at(int frac_digit)
{   /* 2^53+1 (an exact tie) followed by frac_digit-1 zeros and a final 1 at fractional digit frac_digit */
    int i, n = 0; const char *p = "9007199254740993.";
    while (*p) buf[n++] = *p++;
    for (i = 0; i < frac_digit - 1; i++) buf[n++] = '0';
    buf[n++] = '1'; buf[n] = 0;
}
TC_CASE(tie_broken_by_750th_digit)
{   /* observed: a nonzero digit inside the first ~768 significant digits is honoured => strictly above the tie => rounds up to 2^53+2 */
    tie_plus_one_at(750);
    CHECK_BITS_D(strtod(buf, NULL), FB(0x4340000000000001ULL));
}
TC_CASE(tie_not_broken_by_800th_digit)
{   /* observed Win10 22H2 ucrtbase: significant digits beyond ~768 are dropped, so the tie still rounds to even (2^53) although the exact
     * value is just above the tie. C11 only requires correct rounding up to DECIMAL_DIG digits; for longer inputs either neighbour is allowed. */
    tie_plus_one_at(800);
    CHECK_BITS_D(strtod(buf, NULL), FB(0x4340000000000000ULL));
}
TC_CASE(tie_exact_with_trailing_zeros)
{   /* exact tie padded with zeros still ties to even (2^53) */
    int i, n = 0; const char *p = "9007199254740993.";
    while (*p) buf[n++] = *p++;
    for (i = 0; i < 800; i++) buf[n++] = '0';
    buf[n] = 0;
    CHECK_BITS_D(strtod(buf, NULL), FB(0x4340000000000000ULL));
}
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(tenth_800_digits); TC_RUN(tie_broken_by_750th_digit); TC_RUN(tie_not_broken_by_800th_digit); TC_RUN(tie_exact_with_trailing_zeros); return tc_end(); }
