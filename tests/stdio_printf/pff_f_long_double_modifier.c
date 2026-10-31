/* TC
 * id:    pff_f_long_double_modifier
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 p7 (l has no effect on f)
 * note:  %lf takes a double; the %L variants live in pff_f_long_double_modifier_probe
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double d = 1.5;
static char b[96];

TC_CASE(lf) { sprintf(b, "%lf", d); CHECK_STR(b, "1.500000"); }
TC_CASE(lf_mix) { sprintf(b, "%lf %d %lf", d, 7, d); CHECK_STR(b, "1.500000 7 1.500000"); }
int main(void) { tc_begin(); TC_RUN(lf); TC_RUN(lf_mix); return tc_end(); }
