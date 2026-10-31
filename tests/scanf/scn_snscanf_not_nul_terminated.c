/* TC
 * id:    scn_snscanf_not_nul_terminated
 * area:  scanf
 * funcs: _snscanf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snscanf (count bounds the characters read; the source need not be NUL terminated)
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(alpha_digits) { char src[9] = {'a','b','c','1','2','3','x','y','z'}; char w[16]; int n = 0, r; w[0] = 0; cnt = 6; r = _snscanf(src, cnt, "%[a-z]%d", w, &n); CHECK_EQ_I(r, 2); CHECK_STR(w, "abc"); CHECK_EQ_I(n, 123); }
TC_CASE(trailing_letters_not_consumed) { char src[8] = {'a','b','c','d','e','f','g','h'}; char w[16]; int r; w[0] = 0; cnt = 4; r = _snscanf(src, cnt, "%[a-z]", w); CHECK_EQ_I(r, 1); CHECK_STR(w, "abcd"); }
TC_CASE(digits_cut) { char src[6] = {'1','2','3','4','5','6'}; int v = 0, r; cnt = 3; r = _snscanf(src, cnt, "%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 123); }

int main(void) { tc_begin(); TC_RUN(alpha_digits); TC_RUN(trailing_letters_not_consumed); TC_RUN(digits_cut); return tc_end(); }
