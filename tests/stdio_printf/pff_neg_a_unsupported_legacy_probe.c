/* TC
 * id:    pff_neg_a_unsupported_legacy_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field (%a/%A added with VS2013 / msvcr120)
 * note:  PROBE: pre-msvcr120 CRTs have no %a. Only the negative result is asserted: the output must not be the
 *        hex-float text "0x1.8p+0" (the CRT either rejects the format via the invalid-parameter handler or prints junk).
 *        The invalid-parameter handler count is deliberately not asserted (0 or 1). The process must not crash.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static void run(const char *fmt)
{
    char b[64]; volatile double v = 1.5; int i;
    for (i = 0; i < 64; i++) b[i] = 0;
    tc_iph_install(); tc_iph_count = 0;
    sprintf(b, fmt, v);
    b[63] = 0;
    CHECK(strcmp(b, "0x1.8p+0") != 0);
    CHECK(strcmp(b, "0x1.8p+0") != 0 || tc_iph_count == 0);
    CHECK(tc_iph_count == 0 || tc_iph_count == 1);
}
TC_CASE(a_not_hexfloat) { run("%.1a"); }
TC_CASE(A_not_hexfloat) { char b[64]; volatile double v = 1.5; int i; for (i = 0; i < 64; i++) b[i] = 0;
    tc_iph_install(); sprintf(b, "%.1A", v); b[63] = 0; CHECK(strcmp(b, "0X1.8P+0") != 0); }

int main(void) { tc_begin(); TC_RUN(a_not_hexfloat); TC_RUN(A_not_hexfloat); return tc_end(); }
