/* TC
 * id:    pfm_1_star_prec_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4 (* precision); MS docs printf width/precision
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define T(fmt, p, v, want) do { char b[64]; volatile int tp = (p); volatile int tv = (v); \
    int n = sprintf(b, fmt, tp, tv); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(signed_conv) { T("%.*d", 5, 42, "00042"); T("%.*i", 3, -7, "-007"); }
TC_CASE(unsigned_conv) { T("%.*u", 4, 9, "0009"); T("%.*o", 4, 8, "0010"); T("%.*x", 4, 255, "00ff"); T("%.*X", 4, 255, "00FF"); }
TC_CASE(zero_zero) { T("%.*d", 0, 0, ""); T("%.*x", 0, 0, ""); T("%.*d", 0, 5, "5"); }
TC_CASE(prec_smaller_than_digits) { T("%.*d", 2, 12345, "12345"); T("%.*u", 1, 100, "100"); }
TC_CASE(with_width_flags) { T("%8.*d", 4, 42, "    0042"); T("%-8.*d", 4, 42, "0042    "); T("%+.*d", 3, 5, "+005"); }

int main(void) { tc_begin(); TC_RUN(signed_conv); TC_RUN(unsigned_conv); TC_RUN(zero_zero); TC_RUN(prec_smaller_than_digits); TC_RUN(with_width_flags); return tc_end(); }
