/* TC
 * id:    pfm_2_flag_zero_exp_e
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=100
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p6 (0 flag: leading zeros after sign), e/E; MS docs printf flags
 * note:  exponent is 2 digits on ucrtbase, 3 digits on msvcr100/110/120 (default output format)
 */
#include "tc.h"
#include <stdio.h>

static volatile double onehalf = 1.5, mhalf = -1.5;

TC_CASE(zero_pad_pos) { char b[64]; sprintf(b, "%014E", onehalf);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "001.500000E+00");
#else
    CHECK_STR(b, "01.500000E+000");
#endif
}
TC_CASE(zero_pad_neg) { char b[64]; sprintf(b, "%014E", mhalf);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "-01.500000E+00");
#else
    CHECK_STR(b, "-1.500000E+000");
#endif
}
TC_CASE(zero_pad_plus) { char b[64]; sprintf(b, "%+014e", onehalf);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "+01.500000e+00");
#else
    CHECK_STR(b, "+1.500000e+000");
#endif
}
TC_CASE(minus_beats_zero) { char b[64]; sprintf(b, "%-014.1E|", onehalf);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.5E+00       |");
#else
    CHECK_STR(b, "1.5E+000      |");
#endif
}

int main(void) { tc_begin(); TC_RUN(zero_pad_pos); TC_RUN(zero_pad_neg); TC_RUN(zero_pad_plus); TC_RUN(minus_beats_zero); return tc_end(); }
