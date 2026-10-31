/* TC
 * id:    pfv_rounding_half_pre140
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   widely documented MSVC (pre-2015) difference from glibc
 * note:  kind=positive. Old CRT rounds exact decimal halves away from zero. All inputs exactly representable in binary.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(f0_half) { char b[128]; d = 0.5; sprintf(b, "%.0f", d); CHECK_STR(b, "1"); }
TC_CASE(f0_one_half) { char b[128]; d = 1.5; sprintf(b, "%.0f", d); CHECK_STR(b, "2"); }
TC_CASE(f0_two_half) { char b[128]; d = 2.5; sprintf(b, "%.0f", d); CHECK_STR(b, "3"); }
TC_CASE(f1_quarter) { char b[128]; d = 0.25; sprintf(b, "%.1f", d); CHECK_STR(b, "0.3"); }
TC_CASE(f2_eighth) { char b[128]; d = 0.125; sprintf(b, "%.2f", d); CHECK_STR(b, "0.13"); }
TC_CASE(neg_half) { char b[128]; d = -0.5; sprintf(b, "%.0f", d); CHECK_STR(b, "-1"); }

int main(void) { tc_begin(); TC_RUN(f0_half); TC_RUN(f0_one_half); TC_RUN(f0_two_half); TC_RUN(f1_quarter); TC_RUN(f2_eighth); TC_RUN(neg_half); return tc_end(); }
