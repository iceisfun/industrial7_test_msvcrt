/* TC
 * id:    cv_strtod_longdigits_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.3 (result is correctly rounded when the input has more digits than DECIMAL_DIG)
 * note:  Probes whether each CRT honours digits far beyond 17 (800-digit mantissas). The C standard permits
 *        implementations to be slightly off only for inputs with more than DECIMAL_DIG digits; UCRT is known to be
 *        exact, older CRTs unknown.
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
TC_CASE(tie_broken_by_800th_digit)
{   /* 2^53+1 plus 1e-801: strictly above the tie => rounds up to 2^53+2 */
    int i, n = 0; const char *p = "9007199254740993.";
    while (*p) buf[n++] = *p++;
    for (i = 0; i < 800; i++) buf[n++] = '0';
    buf[n++] = '1'; buf[n] = 0;
    CHECK_BITS_D(strtod(buf, NULL), FB(0x4340000000000001ULL));
}
TC_CASE(tie_exact_with_trailing_zeros)
{   /* exact tie padded with zeros still ties to even (2^53) */
    int i, n = 0; const char *p = "9007199254740993.";
    while (*p) buf[n++] = *p++;
    for (i = 0; i < 800; i++) buf[n++] = '0';
    buf[n] = 0;
    CHECK_BITS_D(strtod(buf, NULL), FB(0x4340000000000000ULL));
}
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(tenth_800_digits); TC_RUN(tie_broken_by_800th_digit); TC_RUN(tie_exact_with_trailing_zeros); return tc_end(); }
