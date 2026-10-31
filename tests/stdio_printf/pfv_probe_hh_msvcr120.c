/* TC
 * id:    pfv_probe_hh_msvcr120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh added VS2013)
 * note:  PROBE: msvcr120 (VS2013) supports hh length modifier; %hhd of 300 -> 44, %hhu of 511 -> 255.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(hhd_300) { char b[64]; volatile int v = 300; int n = sprintf(b, "%hhd", v); CHECK_STR(b, "44"); CHECK_EQ_I(n, 2); }
TC_CASE(hhu_511) { char b[64]; volatile int v = 511; int n = sprintf(b, "%hhu", v); CHECK_STR(b, "255"); CHECK_EQ_I(n, 3); }

int main(void) { tc_begin(); TC_RUN(hhd_300); TC_RUN(hhu_511); return tc_end(); }
