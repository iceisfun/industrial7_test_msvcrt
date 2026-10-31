/* TC
 * id:    pfm_3_conv_i_len_bigl_w_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes (L, w)
 * note:  PROBING: %Li %wi %LX %wX with int 42: either the modifier is ignored (42 / 2A) or rejected. Chosen guess: ignored, one int consumed (trailing %d prints 7).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[64];
static volatile int v = 42;
#define BODY_T(f, want) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", v, 7); CHECK_STR(b, want "|7");
TC_CASE(Li) { BODY_T("%Li", "42") }
TC_CASE(wi) { BODY_T("%wi", "42") }
TC_CASE(LX) { BODY_T("%LX", "2A") }
TC_CASE(wX) { BODY_T("%wX", "2A") }
int main(void) { tc_begin(); TC_RUN(Li); TC_RUN(wi); TC_RUN(LX); TC_RUN(wX); return tc_end(); }
