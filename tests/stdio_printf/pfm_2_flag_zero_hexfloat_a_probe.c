/* TC
 * id:    pfm_2_flag_zero_hexfloat_a_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1p6 (0 flag pads after sign / 0x prefix for a/A)
 * note:  PROBE: 0 flag with %A and %a places zeros after the 0X prefix and sign; uses default (exact) precision.
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0;

TC_CASE(zero_A) { char b[64]; sprintf(b, "%024A", one); CHECK_STR(b, "0X00001.0000000000000P+0"); }
TC_CASE(zero_a_plus) { char b[64]; sprintf(b, "%+024a", one); CHECK_STR(b, "+0x0001.0000000000000p+0"); }

int main(void) { tc_begin(); TC_RUN(zero_A); TC_RUN(zero_a_plus); return tc_end(); }
