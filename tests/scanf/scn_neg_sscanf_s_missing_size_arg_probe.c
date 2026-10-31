/* TC
 * id:    scn_neg_sscanf_s_missing_size_arg_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs scanf_s: size is required for c, C, s, S and [ conversions that are not assignment-suppressed
 * note:  PROBE. Assignment-suppressed conversions (%*s, %*c, %*[a-z]) take no destination and therefore no size argument: expected
 *        to work without invoking the invalid-parameter handler, as is %d / %n which take no size.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vs;
TC_CASE(star_s_no_size)   { int x = 0, n; CHECK(tc_iph_install()); tc_iph_count = 0; vs = "abc 12"; n = sscanf_s(vs, "%*s %d", &x); CHECK_EQ_I(n, 1); CHECK_EQ_I(x, 12); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(star_c_no_size)   { int x = 0, n; CHECK(tc_iph_install()); tc_iph_count = 0; vs = "a12"; n = sscanf_s(vs, "%*c%d", &x); CHECK_EQ_I(n, 1); CHECK_EQ_I(x, 12); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(star_set_no_size) { int x = 0, n; CHECK(tc_iph_install()); tc_iph_count = 0; vs = "abc12"; n = sscanf_s(vs, "%*[a-z]%d", &x); CHECK_EQ_I(n, 1); CHECK_EQ_I(x, 12); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(d_n_no_size_needed) { int x = 0, k = -1, n; CHECK(tc_iph_install()); tc_iph_count = 0; vs = "42x"; n = sscanf_s(vs, "%d%n", &x, &k); CHECK_EQ_I(n, 1); CHECK_EQ_I(x, 42); CHECK_EQ_I(k, 2); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_RUN(star_s_no_size); TC_RUN(star_c_no_size); TC_RUN(star_set_no_size); TC_RUN(d_n_no_size_needed); return tc_end(); }
