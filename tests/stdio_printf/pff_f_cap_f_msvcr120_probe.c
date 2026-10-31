/* TC
 * id:    pff_f_cap_f_msvcr120_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   VS2013 CRT C99 conformance
 * note:  probed: msvcr120 accepts %F (C99 printf conformance in VS2013) and prints 1.500000
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v = 1.5;
static char b[64];

TC_CASE(F) { sprintf(b, "%F", v); CHECK_STR(b, "1.500000"); }
int main(void) { tc_begin(); TC_RUN(F); return tc_end(); }
