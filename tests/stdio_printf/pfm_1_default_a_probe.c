/* TC
 * id:    pfm_1_default_a_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field a, A
 * note:  PROBE: %A with default precision prints all 13 hex mantissa digits: 1.0 -> 0X1.0000000000000P+0, 255.0 -> 0X1.FE00000000000P+7,
 *        0.0 -> 0X0.0000000000000P+0.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define T(fmt, v, want) do { char b[64]; volatile double tv = (v); sprintf(b, fmt, tv); CHECK_STR(b, want); } while (0)

TC_CASE(A) { T("%A", 1.0, "0X1.0000000000000P+0"); T("%A", -1.0, "-0X1.0000000000000P+0"); T("%A", 255.0, "0X1.FE00000000000P+7"); }
TC_CASE(zero) { T("%A", 0.0, "0X0.0000000000000P+0"); }

int main(void) { tc_begin(); TC_RUN(A); TC_RUN(zero); return tc_end(); }
