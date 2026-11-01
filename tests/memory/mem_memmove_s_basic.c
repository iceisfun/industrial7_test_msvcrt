/* TC
 * id:    mem_memmove_s_basic
 * area:  memory
 * funcs:
 * procs: memmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs memmove_s
 * note:  procs because libmsvcr80/90 lack it. Test bug fixed: d is char[6]="ZZZZZ" so d[5] is the NUL terminator; memmove_s of
 *        5 bytes leaves d[5]==0, not the old expectation byte Z.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));

TC_CASE(exported) { CHECK(tc_p_memmove_s != NULL); }
TC_CASE(non_overlapping) { char d[6] = "ZZZZZ"; volatile size_t n = 5; if (!tc_p_memmove_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memmove_s(d, 6, "abcde", n), 0); CHECK_MEM(d, "abcde", 6); }
TC_CASE(overlap_dest_below_src) { char b[] = "0123456789"; volatile size_t n = 6; if (!tc_p_memmove_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memmove_s(b, 10, b + 2, n), 0); CHECK_MEM(b, "2345676789", 10); }
TC_CASE(overlap_dest_above_src) { char b[] = "0123456789"; volatile size_t n = 6; if (!tc_p_memmove_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memmove_s(b + 2, 8, b, n), 0); CHECK_MEM(b, "0101234589", 10); }
TC_CASE(zero_count_returns_0) { char d[4] = "abc"; volatile size_t n = 0; if (!tc_p_memmove_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memmove_s(d, 4, "xyz", n), 0); CHECK_MEM(d, "abc", 4); }

int main(void) { tc_begin(); TC_LOAD_PROC(memmove_s); TC_RUN(exported); TC_RUN(non_overlapping); TC_RUN(overlap_dest_below_src); TC_RUN(overlap_dest_above_src); TC_RUN(zero_count_returns_0); return tc_end(); }
