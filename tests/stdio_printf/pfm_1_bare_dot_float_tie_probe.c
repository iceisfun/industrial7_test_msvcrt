/* TC
 * id:    pfm_1_bare_dot_float_tie_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 (exact decimal conversion); ucrt rounds exact ties to even, older CRTs away from zero
 * note:  Exact-tie rounding to even on ucrt arrived with Windows 10 2004 (19041); earlier ucrt builds round half away. PROBE: %.f of exact tie 2.5 -> "2" on ucrt (round-half-even on exact value), "3" on pre-2015 CRTs.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(tie_2_5) {
    char b[16]; volatile double v = 2.5; sprintf(b, "%.f", v);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "2");
#else
    CHECK_STR(b, "3");
#endif
}
int main(void) { tc_begin(); TC_RUN(tie_2_5); return tc_end(); }
