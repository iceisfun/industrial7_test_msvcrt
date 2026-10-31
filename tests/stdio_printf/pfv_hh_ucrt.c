/* TC
 * id:    pfv_hh_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.1p7 (hh, h); MS docs printf size prefixes (hh supported since VS2015 UCRT)
 * note:  Out-of-range signed values converted to signed char/short are implementation-defined in ISO C (6.3.1.3p3); MS truncates modulo 2^N. Unsigned cases are fully standard.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

TC_CASE(hhd_300) { char b[64]; volatile int v = 300; sprintf(b, "%hhd", v); CHECK_STR(b, "44"); }
TC_CASE(hhd_neg_129) { char b[64]; volatile int v = -129; sprintf(b, "%hhd", v); CHECK_STR(b, "127"); }
TC_CASE(hhu_511) { char b[64]; volatile unsigned v = 511; sprintf(b, "%hhu", v); CHECK_STR(b, "255"); }
TC_CASE(hhx_1ff) { char b[64]; volatile unsigned v = 0x1ff; sprintf(b, "%hhx", v); CHECK_STR(b, "ff"); }
TC_CASE(hd_70000) { char b[64]; volatile int v = 70000; sprintf(b, "%hd", v); CHECK_STR(b, "4464"); }
TC_CASE(hu_65537) { char b[64]; volatile unsigned v = 65537; sprintf(b, "%hu", v); CHECK_STR(b, "1"); }

int main(void) { tc_begin(); TC_RUN(hhd_300); TC_RUN(hhd_neg_129); TC_RUN(hhu_511); TC_RUN(hhx_1ff); TC_RUN(hd_70000); TC_RUN(hu_65537); return tc_end(); }
