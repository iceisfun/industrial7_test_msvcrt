/* TC
 * id:    pfm_2_flag_hexfloat_default_prec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 a/A (default precision = exact)
 * note:  PROBE: flags with %a/%A at default precision: MS CRT prints all 13 mantissa digits for double (0x1.0000000000000p+0); # adds nothing.
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0;

TC_CASE(minus_a) { char b[64]; sprintf(b, "%-24a|", one); CHECK_STR(b, "0x1.0000000000000p+0    |"); }
TC_CASE(minus_A) { char b[64]; sprintf(b, "%-24A|", one); CHECK_STR(b, "0X1.0000000000000P+0    |"); }
TC_CASE(plus_a) { char b[64]; sprintf(b, "%+a", one); CHECK_STR(b, "+0x1.0000000000000p+0"); }
TC_CASE(space_a) { char b[64]; sprintf(b, "% a", one); CHECK_STR(b, " 0x1.0000000000000p+0"); }
TC_CASE(hash_A) { char b[64]; sprintf(b, "%#A", one); CHECK_STR(b, "0X1.0000000000000P+0"); }

int main(void) { tc_begin(); TC_RUN(minus_a); TC_RUN(minus_A); TC_RUN(plus_a); TC_RUN(space_a); TC_RUN(hash_A); return tc_end(); }
