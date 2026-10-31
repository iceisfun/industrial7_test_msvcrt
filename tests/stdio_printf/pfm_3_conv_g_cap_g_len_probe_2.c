/* TC
 * id:    pfm_3_conv_g_cap_g_len_probe_2
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes on g/G
 * note:  PROBING: %jg %zg %tg %hG %llG format a double like plain %g/%G and consume one double (alternative outcome on Windows: -1 + invalid parameter handler).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5, s = 0.00001;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.5|7");
#define BODY_U(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", s, 7); CHECK_STR(b, "1E-05|7");
TC_CASE(jg) { BODY_T("%jg") }
TC_CASE(zg) { BODY_T("%zg") }
TC_CASE(tg) { BODY_T("%tg") }
TC_CASE(hG) { BODY_U("%hG") }
TC_CASE(llG) { BODY_U("%llG") }
int main(void) { tc_begin(); TC_RUN(jg); TC_RUN(zg); TC_RUN(tg); TC_RUN(hG); TC_RUN(llG); return tc_end(); }
