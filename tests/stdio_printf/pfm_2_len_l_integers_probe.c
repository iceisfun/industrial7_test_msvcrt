/* TC
 * id:    pfm_2_len_l_integers_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (L only for floating)
 * note:  PROBE: L length modifier with integer conversions is believed to be skipped, consuming an int. If the CRT rejects the format the invalid-parameter handler is stubbed so remaining cases still run.
 */
#include "tc.h"
#include <stdio.h>

static volatile int i5 = 5, i255 = 255, i8 = 8;
static volatile unsigned u7 = 7;

TC_CASE(Ld) { char b[32] = {0}; sprintf(b, "%Ld", i5); CHECK_STR(b, "5"); }
TC_CASE(Lu) { char b[32] = {0}; sprintf(b, "%Lu", u7); CHECK_STR(b, "7"); }
TC_CASE(Lx) { char b[32] = {0}; sprintf(b, "%Lx", i255); CHECK_STR(b, "ff"); }
TC_CASE(Lo) { char b[32] = {0}; sprintf(b, "%Lo", i8); CHECK_STR(b, "10"); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(Ld); TC_RUN(Lu); TC_RUN(Lx); TC_RUN(Lo); return tc_end(); }
