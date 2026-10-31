/* TC
 * id:    pfm_2_flag_hash_hexfloat_a
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p6 (# with a/A: decimal point always produced); MS docs printf flags
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0;

TC_CASE(hash_a0) { char b[64]; sprintf(b, "%#.0a", one); CHECK_STR(b, "0x1.p+0"); }
TC_CASE(hash_A0) { char b[64]; sprintf(b, "%#.0A", one); CHECK_STR(b, "0X1.P+0"); }
TC_CASE(no_hash_a0) { char b[64]; sprintf(b, "%.0a", one); CHECK_STR(b, "0x1p+0"); }

int main(void) { tc_begin(); TC_RUN(hash_a0); TC_RUN(hash_A0); TC_RUN(no_hash_a0); return tc_end(); }
