/* TC
 * id:    pfm_3_conv_cap_e_len_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes (h, ll, w, I64) on e/E
 * note:  PROBING: %hE %llE %wE %I64E on a double argument format like plain %E and consume exactly one double. Legacy CRTs (<140) print 3-digit exponents.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#if TC_AT_LEAST(140)
#define W "1.500000E+00|7"
#else
#define W "1.500000E+000|7"
#endif
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, W);
TC_CASE(hE) { BODY_T("%hE") }
TC_CASE(llE) { BODY_T("%llE") }
TC_CASE(wE) { BODY_T("%wE") }
TC_CASE(I64E) { BODY_T("%I64E") }
int main(void) { tc_begin(); TC_RUN(hE); TC_RUN(llE); TC_RUN(wE); TC_RUN(I64E); return tc_end(); }
