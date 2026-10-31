/* TC
 * id:    pfm_1_len_ul_float_faa_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf (L = long double, 64-bit); F, a, A supported VS2013+
 * note:  PROBES %LF, %La, %LA with a double argument; expected "1.500000", "0x1.0000000000000p+0", "0X1.0000000000000P+0".
 */
#include "tc.h"
#include <stdio.h>

static char b[64];
TC_CASE(LF) { volatile double d = 1.5; sprintf(b, "%LF", d); CHECK_STR(b, "1.500000"); }
TC_CASE(La) { volatile double d = 1.0; sprintf(b, "%La", d); CHECK_STR(b, "0x1.0000000000000p+0"); }
TC_CASE(LA) { volatile double d = 1.0; sprintf(b, "%LA", d); CHECK_STR(b, "0X1.0000000000000P+0"); }

int main(void) { tc_begin(); TC_RUN(LF); TC_RUN(La); TC_RUN(LA); return tc_end(); }
