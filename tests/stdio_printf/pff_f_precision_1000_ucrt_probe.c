/* TC
 * id:    pff_f_precision_1000_ucrt_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1p15; MS docs printf precision
 * note:  PROBE: UCRT honours a %f precision above 512 (here 1000) and emits all digits, trailing zeros for an exactly representable value. Split out of pff_f_precision_512_ucrt because MS docs do not state an upper bound.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(half1000)
{
    char b[1100], w[1100]; volatile double v = 0.5; int n, i;
    n = sprintf(b, "%.1000f", v);
    w[0] = '0'; w[1] = '.'; w[2] = '5'; for (i = 3; i < 1002; i++) w[i] = '0'; w[1002] = 0;
    CHECK_STR(b, w); CHECK_EQ_I(n, 1002);
}

int main(void) { tc_begin(); TC_RUN(half1000); return tc_end(); }
