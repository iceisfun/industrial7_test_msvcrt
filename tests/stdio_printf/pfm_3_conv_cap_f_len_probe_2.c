/* TC
 * id:    pfm_3_conv_cap_f_len_probe_2
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field F, size prefixes
 * note:  PROBING: %F exists on msvcr120 and accepts IF I32F I64F jF zF tF as plain %F on a double (consuming one double).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.25;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.250000|7");
TC_CASE(IF) { BODY_T("%IF") }
TC_CASE(I32F) { BODY_T("%I32F") }
TC_CASE(I64F) { BODY_T("%I64F") }
TC_CASE(jF) { BODY_T("%jF") }
TC_CASE(zF) { BODY_T("%zF") }
TC_CASE(tF) { BODY_T("%tF") }
int main(void) { tc_begin(); TC_RUN(IF); TC_RUN(I32F); TC_RUN(I64F); TC_RUN(jF); TC_RUN(zF); TC_RUN(tF); return tc_end(); }
