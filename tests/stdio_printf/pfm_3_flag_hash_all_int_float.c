/* TC
 * id:    pfm_3_flag_hash_all_int_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p6 (# flag: o raises precision so first digit is 0; x/X prefix 0x/0X for nonzero only; e E f F g G always contain a decimal point; g G keep trailing zeros)
 * note:  %#.0o of 0 is isolated in pfm_3_prec_hash_o_zero_probe. %#F is not asserted (F is absent from pre-UCRT CRTs). Exponents have 3 digits before ucrtbase.
 */
#include "tc.h"
#include <stdio.h>

#if TC_CRT_VER >= 140
#define XE "e+00"
#define XU "E+00"
#else
#define XE "e+000"
#define XU "E+000"
#endif

static volatile unsigned u0 = 0, u255 = 255, u8 = 8;
static volatile double d3 = 3.0, d15 = 1.5, d1 = 1.0, d1e5 = 100000.0, d0 = 0.0;

TC_CASE(zero_value_no_prefix)
{
    char b[64];
    sprintf(b, "[%#x][%#X][%#o]", u0, u0, u0);
    CHECK_STR(b, "[0][0][0]");
}
TC_CASE(nonzero_prefix)
{
    char b[64];
    sprintf(b, "[%#x][%#X][%#o][%#o]", u255, u255, u255, u8);
    CHECK_STR(b, "[0xff][0XFF][0377][010]");
}
TC_CASE(hash_hex_width_prec)
{
    char b[64];
    sprintf(b, "[%#6x][%-#6X][%#.4x][%#08x]", u255, u255, u255, u255);
    CHECK_STR(b, "[  0xff][0XFF  ][0x00ff][0x0000ff]");
}
TC_CASE(forced_point_f)
{
    char b[64];
    sprintf(b, "[%#.0f][%#.0f][%.0f][%#f]", d3, d0, d3, d3);
    CHECK_STR(b, "[3.][0.][3][3.000000]");
}
TC_CASE(forced_point_e)
{
    char b[64];
    sprintf(b, "[%#.0e][%#.0E][%.0e]", d1, d1, d1);
    CHECK_STR(b, "[1." XE "][1." XU "][1" XE "]");
}
TC_CASE(g_keeps_trailing_zeros)
{
    char b[64];
    sprintf(b, "[%#g][%g][%#.3g][%#.0g][%#g]", d15, d15, d1, d1, d0);
    CHECK_STR(b, "[1.50000][1.5][1.00][1.][0.00000]");
}
TC_CASE(cap_g_hundred_thousand)
{
    char b[64];
    sprintf(b, "[%#G][%G]", d1e5, d1e5);
    CHECK_STR(b, "[100000.][100000]");
}

int main(void)
{
    tc_begin();
    TC_RUN(zero_value_no_prefix); TC_RUN(nonzero_prefix); TC_RUN(hash_hex_width_prec); TC_RUN(forced_point_f);
    TC_RUN(forced_point_e); TC_RUN(g_keeps_trailing_zeros); TC_RUN(cap_g_hundred_thousand);
    return tc_end();
}
