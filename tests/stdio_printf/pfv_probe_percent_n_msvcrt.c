/* TC
 * id:    pfv_probe_percent_n_msvcrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented for msvcrt.dll)
 * note:  PROBE: legacy msvcrt.dll honours %n by default: sprintf(b,"abc%n",&n) stores 3, returns 3.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(n_stores_count)
{
    char b[64]; volatile int n = -1; int r = sprintf(b, "abc%n", (int *)&n);
    CHECK_STR(b, "abc"); CHECK_EQ_I(r, 3); CHECK_EQ_I(n, 3);
}
int main(void) { tc_begin(); TC_RUN(n_stores_count); return tc_end(); }
