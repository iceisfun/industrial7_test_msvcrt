/* TC
 * id:    pfi_probe_i32_msvcrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes (I32, I64)
 * note:  PROBE: I32 in Windows 7 msvcrt.dll; I64 is the control in its own case.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f32 = "%I32d"; static char *volatile f64 = "%I64d";
TC_CASE(I32) { char b[64]; sprintf(b, f32, -5); CHECK_STR(b, "-5"); }
TC_CASE(I64) { char b[64]; sprintf(b, f64, (long long)-5); CHECK_STR(b, "-5"); }
int main(void) { tc_begin(); TC_RUN(I32); TC_RUN(I64); return tc_end(); }
