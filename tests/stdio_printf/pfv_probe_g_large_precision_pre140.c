/* TC
 * id:    pfv_probe_g_large_precision_pre140
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   old CRT converts to 17 significant digits then pads
 * note:  PROBE: %.20f 0.1 -> 0.10000000000000001000; %.25f -> 0.1000000000000000100000000; %.30g -> 0.10000000000000001 (trailing zeros stripped).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(f20_point1) { char b[128]; d = 0.1; sprintf(b, "%.20f", d); CHECK_STR(b, "0.10000000000000001000"); }
TC_CASE(f25_point1) { char b[128]; d = 0.1; sprintf(b, "%.25f", d); CHECK_STR(b, "0.1000000000000000100000000"); }
TC_CASE(g30_point1) { char b[128]; d = 0.1; sprintf(b, "%.30g", d); CHECK_STR(b, "0.10000000000000001"); }

int main(void) { tc_begin(); TC_RUN(f20_point1); TC_RUN(f25_point1); TC_RUN(g30_point1); return tc_end(); }
