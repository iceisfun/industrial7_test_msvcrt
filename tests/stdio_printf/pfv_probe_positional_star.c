/* TC
 * id:    pfv_probe_positional_star
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf_p positional parameters (width via *n$)
 * note:  PROBE: "%1$*2$d" with (7, 4) -> "   7" (value is arg 1, width is arg 2).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(width_star_positional) { char b[64]; volatile int v = 7, w = 4; _sprintf_p(b, 64, "%1$*2$d", v, w); CHECK_STR(b, "   7"); }
int main(void) { tc_begin(); TC_RUN(width_star_positional); return tc_end(); }
