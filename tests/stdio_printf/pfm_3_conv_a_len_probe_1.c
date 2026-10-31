/* TC
 * id:    pfm_3_conv_a_len_probe_1
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes on a/A
 * note:  PROBING: %.1La %.1ha %.1hha %.1lla %.1wa %.1Ia format a double like plain %.1a / %.1A (1.5 -> 0x1.8p+0) and consume one double. Explicit precision avoids default-precision uncertainty.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "0x1.8p+0|7");
TC_CASE(La) { BODY_T("%.1La") }
TC_CASE(ha) { BODY_T("%.1ha") }
TC_CASE(hha) { BODY_T("%.1hha") }
TC_CASE(lla) { BODY_T("%.1lla") }
TC_CASE(wa) { BODY_T("%.1wa") }
TC_CASE(Ia) { BODY_T("%.1Ia") }
int main(void) { tc_begin(); TC_RUN(La); TC_RUN(ha); TC_RUN(hha); TC_RUN(lla); TC_RUN(wa); TC_RUN(Ia); return tc_end(); }
