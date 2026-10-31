/* TC
 * id:    pfm_1_len_l_float_faa_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (F, a, A; l ignored); MS docs printf (F, a, A supported VS2013+)
 * note:  PROBES %lF, %la, %lA with a double. Expected: "1.500000", "0x1.0000000000000p+0", "0X1.0000000000000P+0"
 *        (UCRT prints 13 hex digits by default and a minimal exponent). msvcr120's exact %a layout is a guess.
 */
#include "tc.h"
#include <stdio.h>

static char b[64];
TC_CASE(lF) { volatile double d = 1.5; sprintf(b, "%lF", d); CHECK_STR(b, "1.500000"); }
TC_CASE(la) { volatile double d = 1.0; sprintf(b, "%la", d); CHECK_STR(b, "0x1.0000000000000p+0"); }
TC_CASE(lA) { volatile double d = 1.0; sprintf(b, "%lA", d); CHECK_STR(b, "0X1.0000000000000P+0"); }

int main(void) { tc_begin(); TC_RUN(lF); TC_RUN(la); TC_RUN(lA); return tc_end(); }
