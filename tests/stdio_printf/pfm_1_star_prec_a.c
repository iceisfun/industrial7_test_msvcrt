/* TC
 * id:    pfm_1_star_prec_a
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field a, A; C11 7.21.6.1
 * note:  hex-float with explicit * precision on ucrt
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define T(fmt, p, v, want) do { char b[64]; volatile int tp = (p); volatile double tv = (v); \
    int n = sprintf(b, fmt, tp, tv); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(a) { T("%.*a", 2, 1.0, "0x1.00p+0"); }
TC_CASE(A) { T("%.*A", 2, 1.0, "0X1.00P+0"); T("%.*A", 3, 255.0, "0X1.FE0P+7"); }

int main(void) { tc_begin(); TC_RUN(a); TC_RUN(A); return tc_end(); }
