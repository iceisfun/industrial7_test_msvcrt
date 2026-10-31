/* TC
 * id:    pfm_3_conv_cap_f_len_probe_1
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field F, size prefixes
 * note:  PROBING: %F exists on msvcr120 and accepts F LF hF hhF llF wF as plain %F on a double (consuming one double).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.25;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.250000|7");
TC_CASE(F) { BODY_T("%F") }
TC_CASE(LF) { BODY_T("%LF") }
TC_CASE(hF) { BODY_T("%hF") }
TC_CASE(hhF) { BODY_T("%hhF") }
TC_CASE(llF) { BODY_T("%llF") }
TC_CASE(wF) { BODY_T("%wF") }
int main(void) { tc_begin(); TC_RUN(F); TC_RUN(LF); TC_RUN(hF); TC_RUN(hhF); TC_RUN(llF); TC_RUN(wF); return tc_end(); }
