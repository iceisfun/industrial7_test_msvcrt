/* TC
 * id:    scn_neg_sscanf_suppress_missing_arg_ok
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p10 (assignment-suppressing *), p16 (return = number of items assigned)
 * note:  Suppressed conversions consume no pointer argument and are not counted, so the call is valid with no varargs.
 *        Inputs have trailing text so end-of-input is never reached (avoids the EOF-vs-0 question).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

TC_CASE(star_d_none)   { int r, n = -1; in = "5 x"; r = sscanf(in, "%*d%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 1); }
TC_CASE(star_s_twice)  { int r; in = "a b c"; r = sscanf(in, "%*s%*s"); CHECK_EQ_I(r, 0); }
TC_CASE(star_then_one) { int x = 0, r; in = "1 2"; r = sscanf(in, "%*d %d", &x); CHECK_EQ_I(r, 1); CHECK_EQ_I(x, 2); }
TC_CASE(star_set)      { int r, n = -1; in = "abc1"; r = sscanf(in, "%*[a-z]%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 3); }
TC_CASE(star_c)        { int r, n = -1; in = "abc"; r = sscanf(in, "%*2c%n", &n); CHECK_EQ_I(r, 0); CHECK_EQ_I(n, 2); }

int main(void) { tc_begin(); TC_RUN(star_d_none); TC_RUN(star_s_twice); TC_RUN(star_then_one); TC_RUN(star_set); TC_RUN(star_c); return tc_end(); }
