/* TC
 * id:    pfv_probe_hexfloat_denormal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none (C leaves subnormal %a form unspecified)
 * note:  PROBE: MS prints subnormals unnormalised: 0x0.0000000000001p-1022 for the smallest double.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64];
TC_CASE(denormal_min) { volatile double d = 4.9406564584124654e-324; sprintf(b, "%a", d); CHECK_STR(b, "0x0.0000000000001p-1022"); }

int main(void) { tc_begin(); TC_RUN(denormal_min); return tc_end(); }
