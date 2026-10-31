/* TC
 * id:    pfm_1_len_ul_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf; Microsoft long double is 64-bit (same as double), so the CRT reads a double for %L[efg]
 * note:  A plain double is passed for %L conversions, matching Microsoft's 64-bit long double. Pre-UCRT prints three exponent digits.
 *        The mingw compiler is told nothing about this: arguments are volatile doubles read through the format.
 */
#include "tc.h"
#include <stdio.h>

#if TC_CRT_VER >= 140
#define E0 "e+00"
#define UE0 "E+00"
#else
#define E0 "e+000"
#define UE0 "E+000"
#endif
static char b[64];
TC_CASE(Le) { volatile double d = 1.5; sprintf(b, "%Le", d); CHECK_STR(b, "1.500000" E0); }
TC_CASE(LE) { volatile double d = 1.5; sprintf(b, "%LE", d); CHECK_STR(b, "1.500000" UE0); }
TC_CASE(Lf) { volatile double d = 1.5; sprintf(b, "%Lf", d); CHECK_STR(b, "1.500000"); }
TC_CASE(Lg) { volatile double d = 0.25; sprintf(b, "%Lg", d); CHECK_STR(b, "0.25"); }

int main(void) { tc_begin(); TC_RUN(Le); TC_RUN(LE); TC_RUN(Lf); TC_RUN(Lg); return tc_end(); }
