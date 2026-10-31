/* TC
 * id:    pfm_1_prec_num_a_none
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf type field a, A; C11 7.21.6.1 (A)
 * note:  msvcr120 %a output format not certain, so ucrtbase only; explicit precision only; default-precision forms are in pfm_1_default_A_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define T(fmt, v, want) do { char b[64]; volatile double tv = (v); int n = sprintf(b, fmt, tv); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(A) { T("%.3A", 1.0, "0X1.000P+0"); T("%.3A", 255.0, "0X1.FE0P+7"); T("%.3A", -1.0, "-0X1.000P+0"); }
TC_CASE(zero) { T("%.3A", 0.0, "0X0.000P+0"); }

int main(void) { tc_begin(); TC_RUN(A); TC_RUN(zero); return tc_end(); }
