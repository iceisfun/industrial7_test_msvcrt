/* TC
 * id:    pfm_2_len_hh_float_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1p7 (h/hh only for integer conversions; otherwise undefined)
 * note:  PROBE: h/hh with floating conversions is believed ignored (double still consumed). If the CRT rejects the format the invalid-parameter handler is stubbed so remaining cases still run.
 */
#include "tc.h"
#include <stdio.h>

static volatile double onehalf = 1.5, one = 1.0;

TC_CASE(hf) { char b[32] = {0}; sprintf(b, "%hf", onehalf); CHECK_STR(b, "1.500000"); }
TC_CASE(hhe) { char b[32] = {0}; sprintf(b, "%hhe", onehalf); CHECK_STR(b, "1.500000e+00"); }
TC_CASE(hg) { char b[32] = {0}; sprintf(b, "%hg", onehalf); CHECK_STR(b, "1.5"); }
TC_CASE(hhA) { char b[32] = {0}; sprintf(b, "%hhA", one); CHECK_STR(b, "0X1.0000000000000P+0"); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(hf); TC_RUN(hhe); TC_RUN(hg); TC_RUN(hhA); return tc_end(); }
