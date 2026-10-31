/* TC
 * id:    pfm_1_len_hh_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh listed for VS2013+/UCRT)
 * note:  PROBES whether msvcr120 honours the C99 hh length modifier (expected: yes, 0x1ff -> "ff"). msvcr110/100 deliberately
 *        not asserted (see pfv_neg_probe_hh_pre120).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(hhx) { char b[32]; volatile int v = 0x1ff; sprintf(b, "%hhx", v); CHECK_STR(b, "ff"); }
TC_CASE(hhd) { char b[32]; volatile int v = 200;   sprintf(b, "%hhd", v); CHECK_STR(b, "-56"); }

int main(void) { tc_begin(); TC_RUN(hhx); TC_RUN(hhd); return tc_end(); }
