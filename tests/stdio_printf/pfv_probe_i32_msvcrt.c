/* TC
 * id:    pfv_probe_i32_msvcrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes I32
 * note:  PROBE: legacy msvcrt.dll accepts the I32 size prefix (same as no prefix for 32-bit int).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(I32d) { char b[64]; volatile int v = -7; sprintf(b, "%I32d", v); CHECK_STR(b, "-7"); }
TC_CASE(I32u) { char b[64]; volatile unsigned int v = 4294967295u; sprintf(b, "%I32u", v); CHECK_STR(b, "4294967295"); }

int main(void) { tc_begin(); TC_RUN(I32d); TC_RUN(I32u); return tc_end(); }
