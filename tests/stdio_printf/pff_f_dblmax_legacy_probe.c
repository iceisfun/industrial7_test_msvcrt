/* TC
 * id:    pff_f_dblmax_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf (legacy CRT)
 * note:  probed: legacy CRT emits 17 significant digits then zero-fills for huge values
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v_max = 1.7976931348623157e308;
static char big[1024];

TC_CASE(length) { int n = sprintf(big, "%.0f", v_max); CHECK_EQ_I(n, 309); }
TC_CASE(prefix) { sprintf(big, "%.0f", v_max); CHECK(memcmp(big, "17976931348623157", 17) == 0); }
TC_CASE(zero_fill)
{
    size_t i, n; sprintf(big, "%.0f", v_max); n = strlen(big);
    for (i = 17; i < n; i++) if (big[i] != '0') { CHECK_EQ_I(i, -1); break; }
}
int main(void) { tc_begin(); TC_RUN(length); TC_RUN(prefix); TC_RUN(zero_fill); return tc_end(); }
