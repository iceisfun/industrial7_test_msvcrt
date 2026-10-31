/* TC
 * id:    pff_f_long_double_modifier_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf Size prefixes (L = long double, same as double)
 * note:  probed: %L with e/f/g consumes a plain double (long double == double on MSVC) on every CRT; split from the %lf case because MS docs do not clearly list L for printf. legacy CRTs print 3-digit exponents, UCRT 2-digit. long double is 8 bytes on MSVC; a double argument is passed (not mingw long double)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double d = 1.5;
static char b[96];

TC_CASE(Lf) { sprintf(b, "%Lf", d); CHECK_STR(b, "1.500000"); }
TC_CASE(Le_Lg)
{
    sprintf(b, "%Le|%Lg|%LG", d, d, d);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.500000e+00|1.5|1.5");   /* UCRT: two-digit exponent minimum */
#else
    CHECK_STR(b, "1.500000e+000|1.5|1.5");  /* legacy CRT: three-digit exponent by default */
#endif
}
TC_CASE(mix) { sprintf(b, "%Lf %d %Lg %d", d, 7, d, 9); CHECK_STR(b, "1.500000 7 1.5 9"); }
int main(void) { tc_begin(); TC_RUN(Lf); TC_RUN(Le_Lg); TC_RUN(mix); return tc_end(); }
