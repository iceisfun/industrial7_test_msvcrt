/* TC
 * id:    scn_sscanf_embedded_nul
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 (string input ends at the terminating NUL); MS docs sscanf
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(nul_between) { static const char in[] = "12\0 34"; int a = 0, b = 9, r = sscanf(in, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 9); }
TC_CASE(leading_nul) { static const char in[] = "\0abc"; char s[8] = "zz"; int r = sscanf(in, "%s", s); CHECK_EQ_I(r, EOF); CHECK_STR(s, "zz"); }
TC_CASE(c_at_nul) { static const char in[] = "\0abc"; char c = 'z'; int r = sscanf(in, "%c", &c); CHECK_EQ_I(r, EOF); CHECK_EQ_I(c, 'z'); }

int main(void) { tc_begin(); TC_RUN(nul_between); TC_RUN(leading_nul); TC_RUN(c_at_nul); return tc_end(); }
