/* TC
 * id:    cv_strtod_halfway_long_ucrt
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * note:  PROBE: >17 significant digits, where C11 5.2.4.2.2 does not mandate correct rounding; UCRT is believed to be exact. Inputs with 20-55 significant digits at an exact binary tie; ucrtbase only (older CRTs truncate to ~21 digits).
 * ref:   C11 7.22.1.3 / 5.2.4.2.2: correctly rounded, ties to even in the default rounding mode; IEEE 754
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static volatile const char *vs;
static double cv(const char *s) { vs = s; return tc_p_strtod((const char *)vs, NULL); }

TC_CASE(one_plus_half_ulp)
{
    /* 1 + 2^-53 is exactly halfway between 1 and 1+2^-52: ties to even -> 1.0 */
    CHECK_BITS_D(cv("1.00000000000000011102230246251565404236316680908203125"), 1.0);
    CHECK_BITS_D(cv("1.00000000000000011102230246251565404236316680908203124"), 1.0);
}

TC_CASE(one_plus_half_ulp_plus)
{
    CHECK_BITS_D(cv("1.00000000000000011102230246251565404236316680908203126"), 1.0000000000000002220446049250313);
    CHECK_BITS_D(cv("1.000000000000000111022302462515654042363166809082031251"), 1.0000000000000002220446049250313);
}

TC_CASE(three_halves_ulp)
{
    /* 1 + 3*2^-53 is halfway between 1+2^-52 (odd) and 1+2^-51 (even): -> 1+2^-51 */
    CHECK_BITS_D(cv("1.00000000000000033306690738754696212708950042724609375"), 1.0000000000000004440892098500626);
}

TC_CASE(long_tail_over_tie)
{
    CHECK_BITS_D(cv("9007199254740993.000001"), 9007199254740994.0);
    CHECK_BITS_D(cv("4503599627370497.4999"), 4503599627370497.0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(one_plus_half_ulp); TC_RUN(one_plus_half_ulp_plus); TC_RUN(three_halves_ulp); TC_RUN(long_tail_over_tie);
    return tc_end();
}
