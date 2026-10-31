/* TC
 * id:    pfm_3_conv_g_cap_g_len_probe_1
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes on g/G
 * note:  PROBING: %hhg %llg %wg %Ig %I64g format a double like plain %g/%G and consume one double (alternative outcome on Windows: -1 + invalid parameter handler).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5, s = 0.00001;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.5|7");
#define BODY_U(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", s, 7); CHECK_STR(b, "1E-05|7");
TC_CASE(hhg) { BODY_T("%hhg") }
TC_CASE(llg) { BODY_T("%llg") }
TC_CASE(wg) { BODY_T("%wg") }
TC_CASE(Ig) { BODY_T("%Ig") }
TC_CASE(I64g) { BODY_T("%I64g") }
int main(void) { tc_begin(); TC_RUN(hhg); TC_RUN(llg); TC_RUN(wg); TC_RUN(Ig); TC_RUN(I64g); return tc_end(); }
