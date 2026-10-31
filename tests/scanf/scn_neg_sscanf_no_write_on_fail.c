/* TC
 * id:    scn_neg_sscanf_no_write_on_fail
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p16 (matching failure stops; later arguments not written)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(int_fail_untouched) { int a = 0x1234, r = sscanf("zzz", "%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 0x1234); }
TC_CASE(second_not_written) { int a = 0, b = 0x55, r = sscanf("4 q", "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(b, 0x55); }
TC_CASE(sign_only) { int a = 42, r = sscanf("-", "%d", &a); CHECK(r != 1); CHECK_EQ_I(a, 42); }

int main(void) { tc_begin(); TC_RUN(int_fail_untouched); TC_RUN(second_not_written); TC_RUN(sign_only); return tc_end(); }
