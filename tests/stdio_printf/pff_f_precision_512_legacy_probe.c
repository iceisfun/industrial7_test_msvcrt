/* TC
 * id:    pff_f_precision_512_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf precision (legacy CRT)
 * note:  probed: legacy CRT supports precision 512 on %f; whether larger precision is clamped is deliberately NOT asserted
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v_half = 0.5;
static char big[2048];

TC_CASE(prec512)
{
    int n, i; sprintf(big, "%.512f", v_half); n = (int)strlen(big);
    CHECK_EQ_I(n, 514);
    CHECK(memcmp(big, "0.5", 3) == 0);
    for (i = 3; i < n; i++) if (big[i] != '0') { CHECK_EQ_I(i, -1); break; }
}
TC_CASE(prec100)
{
    int n, i; sprintf(big, "%.100f", v_half); n = (int)strlen(big);
    CHECK_EQ_I(n, 102);
    CHECK(memcmp(big, "0.5", 3) == 0);
    for (i = 3; i < n; i++) if (big[i] != '0') { CHECK_EQ_I(i, -1); break; }
}
int main(void) { tc_begin(); TC_RUN(prec512); TC_RUN(prec100); return tc_end(); }
