/* TC
 * id:    scn_sscanf_inf_nan_trailing_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C99 7.19.6.2 fscanf; nan(n-char-sequence)
 * note:  PROBE: whether UCRT scanf consumes the parenthesised n-char-sequence after nan, and whether 'infx' leaves 'x' unread after inf (C99 says yes to both).
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
#define INFB 0x7FF0000000000000ULL

TC_CASE(nan_payload_consumed) { double d = 0; int c = -1, r; v = "nan(123)";
    r = sscanf((const char *)v, "%lf%n", &d, &c); CHECK_EQ_I(r, 1); CHECK_IS_NAN(d); CHECK_EQ_I(c, 8); }
TC_CASE(nan_payload_then_char) { double d = 0; char ch = 0; int r; v = "nan(abc_1)z";
    r = sscanf((const char *)v, "%lf%c", &d, &ch); CHECK_EQ_I(r, 2); CHECK_IS_NAN(d); CHECK_EQ_I(ch, 'z'); }
TC_CASE(inf_then_x) { double d = 0; char ch = 0; int r; v = "infx";
    r = sscanf((const char *)v, "%lf%c", &d, &ch); CHECK_EQ_I(r, 2); CHECK_EQ_U(tc_dbits(d), INFB); CHECK_EQ_I(ch, 'x'); }
int main(void) { tc_begin(); TC_RUN(nan_payload_consumed); TC_RUN(nan_payload_then_char); TC_RUN(inf_then_x); return tc_end(); }
