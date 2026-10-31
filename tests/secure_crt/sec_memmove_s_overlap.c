/* TC
 * id:    sec_memmove_s_overlap
 * area:  secure_crt
 * funcs: memmove_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs memmove_s
 */
#include "tc.h"
#include <string.h>

static volatile size_t v_n;

TC_CASE(forward_overlap) { char b[8] = "abcdefg"; v_n = 5; CHECK_EQ_I(memmove_s(b + 2, 6, b, v_n), 0); CHECK_MEM(b, "ababcde", 7); }
TC_CASE(backward_overlap) { char b[8] = "abcdefg"; v_n = 5; CHECK_EQ_I(memmove_s(b, 8, b + 2, v_n), 0); CHECK_MEM(b, "cdefgfg", 7); }
TC_CASE(same_pointer) { char b[4] = "abc"; v_n = 3; CHECK_EQ_I(memmove_s(b, 4, b, v_n), 0); CHECK_STR(b, "abc"); }
TC_CASE(count_zero) { char b[4] = "abc"; v_n = 0; CHECK_EQ_I(memmove_s(b, 4, b + 1, v_n), 0); CHECK_STR(b, "abc"); }
TC_CASE(disjoint) { char s[4] = "xyz", d[4] = "...";  v_n = 3; CHECK_EQ_I(memmove_s(d, 4, s, v_n), 0); CHECK_STR(d, "xyz"); }

int main(void) { tc_begin(); TC_RUN(forward_overlap); TC_RUN(backward_overlap); TC_RUN(same_pointer); TC_RUN(count_zero); TC_RUN(disjoint); return tc_end(); }
