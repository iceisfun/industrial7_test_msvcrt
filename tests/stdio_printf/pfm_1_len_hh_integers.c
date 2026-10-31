/* TC
 * id:    pfm_1_len_hh_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1p7 (hh); MS docs printf Type Field Characters / Size Prefixes (hh = signed/unsigned char)
 * note:  hh narrows the promoted int argument to (un)signed char before formatting. Pre-UCRT runtimes are covered by pfm_1_len_hh_legacy_probe.
 */
#include "tc.h"
#include <stdio.h>

static char b[64];
TC_CASE(hhi)  { volatile int v = 0x1ff; sprintf(b, "%hhi", v); CHECK_STR(b, "-1"); }
TC_CASE(hhd)  { volatile int v = 200;   sprintf(b, "%hhd", v); CHECK_STR(b, "-56"); }
TC_CASE(hhx)  { volatile int v = 0x1ff; sprintf(b, "%hhx", v); CHECK_STR(b, "ff"); }
TC_CASE(hhX)  { volatile int v = 0x1ab; sprintf(b, "%hhX", v); CHECK_STR(b, "AB"); }
TC_CASE(hhu)  { volatile int v = 0x101; sprintf(b, "%hhu", v); CHECK_STR(b, "1"); }
TC_CASE(hho)  { volatile int v = 0x1ff; sprintf(b, "%hho", v); CHECK_STR(b, "377"); }
TC_CASE(hhu_neg) { volatile int v = -1; sprintf(b, "%hhu", v); CHECK_STR(b, "255"); }

int main(void)
{
    tc_begin();
    TC_RUN(hhi); TC_RUN(hhd); TC_RUN(hhx); TC_RUN(hhX); TC_RUN(hhu); TC_RUN(hho); TC_RUN(hhu_neg);
    return tc_end();
}
