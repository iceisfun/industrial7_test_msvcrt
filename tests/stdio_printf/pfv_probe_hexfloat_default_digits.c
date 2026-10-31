/* TC
 * id:    pfv_probe_hexfloat_default_digits
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf a,A (default precision unspecified)
 * note:  PROBE: MS prints 13 hex fraction digits for double %a.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64];
TC_CASE(one) { volatile double d = 1.0; sprintf(b, "%a", d); CHECK_STR(b, "0x1.0000000000000p+0"); }
TC_CASE(half) { volatile double d = 0.5; sprintf(b, "%a", d); CHECK_STR(b, "0x1.0000000000000p-1"); }
TC_CASE(zero) { volatile double d = 0.0; sprintf(b, "%a", d); CHECK_STR(b, "0x0.0000000000000p+0"); }

int main(void) { tc_begin(); TC_RUN(one); TC_RUN(half); TC_RUN(zero); return tc_end(); }
