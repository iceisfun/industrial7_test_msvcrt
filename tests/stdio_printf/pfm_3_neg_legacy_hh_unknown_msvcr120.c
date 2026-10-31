/* TC
 * id:    pfm_3_neg_legacy_hh_unknown_msvcr120
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Size prefixes (hh, j, z, t are C99 additions supported from VS2013 / UCRT)
 * note:  PROBE: the C99 length modifiers hh, j, z, t are NOT understood before msvcr120. Exactly what a legacy CRT prints
 *        (literal text, empty output, or an invalid-parameter call) is not documented, so each case asserts only that the
 *        result differs from the C99 result a UCRT would produce. The invalid-parameter handler is installed where it exists
 *        so that an unknown conversion cannot terminate the process. The test name keeps the historical msvcr120 suffix;
 *        msvcr120 itself is covered by the pfm_1_len_*_probe tests.
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

static char b[64];
TC_CASE(hhd) { volatile int v = 200; b[0] = 0; sprintf(b, "%hhd", v); CHECK(!tc_strequ(b, "-56")); }
TC_CASE(jd)  { volatile long long v = 5000000000LL; b[0] = 0; sprintf(b, "%jd", v); CHECK(!tc_strequ(b, "5000000000")); }
TC_CASE(zd)  { volatile size_t v = 77; b[0] = 0; sprintf(b, "%zd", v); CHECK(!tc_strequ(b, "77")); }
TC_CASE(td)  { volatile ptrdiff_t v = -9; b[0] = 0; sprintf(b, "%td", v); CHECK(!tc_strequ(b, "-9")); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(hhd); TC_RUN(jd); TC_RUN(zd); TC_RUN(td); return tc_end(); }
