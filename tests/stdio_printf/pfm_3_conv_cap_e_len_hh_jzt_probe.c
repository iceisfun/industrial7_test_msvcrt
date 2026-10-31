/* TC
 * id:    pfm_3_conv_cap_e_len_hh_jzt_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh, j, z, t)
 * note:  PROBING: %hhE %jE %zE %tE on a double argument format like plain %E and consume exactly one double.
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
TC_CASE(hhE) { BODY_T("%hhE") }
TC_CASE(jE) { BODY_T("%jE") }
TC_CASE(zE) { BODY_T("%zE") }
TC_CASE(tE) { BODY_T("%tE") }
int main(void) { tc_begin(); TC_RUN(hhE); TC_RUN(jE); TC_RUN(zE); TC_RUN(tE); return tc_end(); }
