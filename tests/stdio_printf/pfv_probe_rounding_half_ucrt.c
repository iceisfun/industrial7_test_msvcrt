/* TC
 * id:    pfv_probe_rounding_half_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   UCRT printf (exactly-representable halves)
 * note:  PROBE: newer UCRT (Win10 2004+) rounds exact halves to even: %.0f 0.5 -> 0; 2.5 -> 2; %.1f 0.25 -> 0.2; %.2f 0.125 -> 0.12; 0.375 -> 0.38. Older UCRT builds round half away from zero. The result identifies the behaviour of each test machine.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(f0_half) { char b[128]; d = 0.5; sprintf(b, "%.0f", d); CHECK_STR(b, "0"); }
TC_CASE(f0_two_half) { char b[128]; d = 2.5; sprintf(b, "%.0f", d); CHECK_STR(b, "2"); }
TC_CASE(f1_quarter) { char b[128]; d = 0.25; sprintf(b, "%.1f", d); CHECK_STR(b, "0.2"); }
TC_CASE(f2_eighth) { char b[128]; d = 0.125; sprintf(b, "%.2f", d); CHECK_STR(b, "0.12"); }
TC_CASE(f2_three_eighths) { char b[128]; d = 0.375; sprintf(b, "%.2f", d); CHECK_STR(b, "0.38"); }

int main(void) { tc_begin(); TC_RUN(f0_half); TC_RUN(f0_two_half); TC_RUN(f1_quarter); TC_RUN(f2_eighth); TC_RUN(f2_three_eighths); return tc_end(); }
