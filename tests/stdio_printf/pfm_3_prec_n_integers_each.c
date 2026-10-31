/* TC
 * id:    pfm_3_prec_n_integers_each
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4 (zero value with zero precision produces no characters); MS docs printf precision specification
 * note:  A bare '.' means precision 0. "%#.0o" of 0 (which C requires to give "0") is isolated in pfm_3_prec_hash_o_zero_probe.
 */
#include "tc.h"
#include <stdio.h>

static volatile int v0 = 0, v7 = 7, v255 = 255;
static volatile unsigned u0 = 0, u9 = 9;

TC_CASE(zero_prec_zero_each)
{
    char b[64]; int n;
    n = sprintf(b, "[%.0u][%.0o][%.0x][%.0X][%.0d][%.0i]", u0, u0, u0, u0, v0, v0);
    CHECK_STR(b, "[][][][][][]"); CHECK_EQ_I(n, 12);
}
TC_CASE(bare_dot_zero_each)
{
    char b[64]; int n;
    n = sprintf(b, "[%.u][%.o][%.x][%.X][%.d][%.i]", u0, u0, u0, u0, v0, v0);
    CHECK_STR(b, "[][][][][][]"); CHECK_EQ_I(n, 12);
}
TC_CASE(bare_dot_nonzero_each)
{
    char b[64];
    sprintf(b, "[%.u][%.x][%.X][%.o][%.d]", u9, v7, v255, v7, v7);
    CHECK_STR(b, "[9][7][FF][7][7]");
}
TC_CASE(prec_n_pads_each)
{
    char b[96];
    sprintf(b, "[%.3u][%.3o][%.3x][%.3X][%.3d][%.3i]", u9, u9, u9, v255, v7, v7);
    CHECK_STR(b, "[009][011][009][0FF][007][007]");
}
TC_CASE(zero_value_width_only)
{
    char b[64];
    sprintf(b, "[%4.0u][%-4.0x][%4.0d][%4.1x]", u0, u0, v0, u0);
    CHECK_STR(b, "[    ][    ][    ][   0]");
}
TC_CASE(prec_smaller_than_digits)
{
    char b[64];
    sprintf(b, "[%.1u][%.2x][%.1o]", 1234u, 0xabcdu, 0777u);
    CHECK_STR(b, "[1234][abcd][777]");
}

int main(void)
{
    tc_begin();
    TC_RUN(zero_prec_zero_each); TC_RUN(bare_dot_zero_each); TC_RUN(bare_dot_nonzero_each);
    TC_RUN(prec_n_pads_each); TC_RUN(zero_value_width_only); TC_RUN(prec_smaller_than_digits);
    return tc_end();
}
