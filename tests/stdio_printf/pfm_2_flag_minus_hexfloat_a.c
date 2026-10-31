/* TC
 * id:    pfm_2_flag_minus_hexfloat_a
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 a/A, flags - + space; MS docs printf type field a/A
 * note:  explicit precision 0 avoids default-precision differences
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0;

TC_CASE(minus_a) { char b[64]; sprintf(b, "%-12.0a|", one); CHECK_STR(b, "0x1p+0      |"); }
TC_CASE(minus_A) { char b[64]; sprintf(b, "%-12.0A|", one); CHECK_STR(b, "0X1P+0      |"); }
TC_CASE(plus_a) { char b[64]; sprintf(b, "%+.0a", one); CHECK_STR(b, "+0x1p+0"); }
TC_CASE(space_a) { char b[64]; sprintf(b, "% .0a", one); CHECK_STR(b, " 0x1p+0"); }

int main(void) { tc_begin(); TC_RUN(minus_a); TC_RUN(minus_A); TC_RUN(plus_a); TC_RUN(space_a); return tc_end(); }
