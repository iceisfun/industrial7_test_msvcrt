/* TC
 * id:    pfm_3_prec_float_matrix_each
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4,p8 (precision for e E f g G; g with precision 0 is treated as 1); MS docs printf precision specification
 * note:  Values avoid exact decimal ties (2.4, 3.6) because tie rounding differs between CRT generations. Exponents have 3 digits before
 *        ucrtbase (msvcr80..120, msvcrt) and 2 on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>

#if TC_CRT_VER >= 140
#define X2 "e+00"
#define X3 "e+03"
#define X6 "e+06"
#define XU2 "E+00"
#define XU3 "E+03"
#define XU6 "E+06"
#define X02 "e+02"
#else
#define X2 "e+000"
#define X3 "e+003"
#define X6 "e+006"
#define XU2 "E+000"
#define XU3 "E+003"
#define XU6 "E+006"
#define X02 "e+002"
#endif

static volatile double d24 = 2.4, d36 = 3.6, d1k = 1234.0, d1m = 1234567.0, dh = 0.5, d123 = 123.0, d7 = 7.0, d15 = 1.5;
static volatile int star3 = 3, star0 = 0;

TC_CASE(bare_dot_values)
{
    char b[96];
    sprintf(b, "[%.e][%.f][%.g][%.E][%.G]", d24, d36, d36, d36, d36);
    CHECK_STR(b, "[2" X2 "][4][4][4" XU2 "][4]");
}
TC_CASE(prec_zero_matches_bare)
{
    char a[96], c[96];
    sprintf(a, "[%.e][%.f][%.g][%.E][%.G]", d24, d36, d36, d36, d36);
    sprintf(c, "[%.0e][%.0f][%.0g][%.0E][%.0G]", d24, d36, d36, d36, d36);
    CHECK_STR(a, c);
}
TC_CASE(star_three)
{
    char b[128];
    sprintf(b, "[%.*e][%.*E][%.*g][%.*G]", star3, d15, star3, d1k, star3, d1k, star3, d1m);
    CHECK_STR(b, "[1.500" X2 "][1.234" XU3 "][1.23" X3 "][1.23" XU6 "]");
}
TC_CASE(star_zero)
{
    char b[128];
    sprintf(b, "[%.*e][%.*f][%.*g]", star0, d24, star0, d36, star0, d36);
    CHECK_STR(b, "[2" X2 "][4][4]");
}
TC_CASE(g_prec_zero_is_one_digit)
{
    char b[128];
    sprintf(b, "[%.0g][%.0g][%.0G][%.1g]", d123, dh, d7, d123);
    CHECK_STR(b, "[1" X02 "][0.5][7][1" X02 "]");
}

int main(void)
{
    tc_begin();
    TC_RUN(bare_dot_values); TC_RUN(prec_zero_matches_bare); TC_RUN(star_three); TC_RUN(star_zero); TC_RUN(g_prec_zero_is_one_digit);
    return tc_end();
}
