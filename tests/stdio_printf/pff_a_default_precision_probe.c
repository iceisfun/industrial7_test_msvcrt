/* TC
 * id:    pff_a_default_precision_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf precision (default for a is 13 hex digits per MS docs?)
 * note:  PROBING: UCRT %a without precision prints all 13 hex fraction digits of a double (not the shortest form glibc prints).
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0, half = 0.5, z = 0.0, three = 3.0;

TC_CASE(one) { char b[64]; sprintf(b, "%a", one); CHECK_STR(b, "0x1.0000000000000p+0"); }
TC_CASE(half) { char b[64]; sprintf(b, "%a", half); CHECK_STR(b, "0x1.0000000000000p-1"); }
TC_CASE(zero) { char b[64]; sprintf(b, "%a", z); CHECK_STR(b, "0x0.0000000000000p+0"); }
TC_CASE(three) { char b[64]; sprintf(b, "%a", three); CHECK_STR(b, "0x1.8000000000000p+1"); }

int main(void) { tc_begin(); TC_RUN(one); TC_RUN(half); TC_RUN(zero); TC_RUN(three); return tc_end(); }
