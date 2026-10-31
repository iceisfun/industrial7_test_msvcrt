/* TC
 * id:    mem_memcpy_s_basic
 * area:  memory
 * funcs:
 * procs: memcpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs memcpy_s
 * note:  procs because libmsvcr80/90 lack it
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));

TC_CASE(exported) { CHECK(tc_p_memcpy_s != NULL); }
TC_CASE(exact_fit_returns_0) { char d[5] = {0}; volatile size_t n = 5; if (!tc_p_memcpy_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memcpy_s(d, 5, "abcde", n), 0); CHECK_MEM(d, "abcde", 5); }
TC_CASE(dest_larger_than_count) { char d[8]; int i; volatile size_t n = 3; if (!tc_p_memcpy_s) TC_SKIP("x"); for (i = 0; i < 8; i++) d[i] = 'Z'; CHECK_EQ_I(tc_p_memcpy_s(d, 8, "abcdef", n), 0); CHECK_MEM(d, "abcZZZZZ", 8); }
TC_CASE(zero_count_returns_0) { char d[4] = "abc"; volatile size_t n = 0; if (!tc_p_memcpy_s) TC_SKIP("x"); CHECK_EQ_I(tc_p_memcpy_s(d, 4, "xyz", n), 0); CHECK_MEM(d, "abc", 4); }
TC_CASE(only_count_bytes_written) { char d[6]; volatile size_t n = 2; if (!tc_p_memcpy_s) TC_SKIP("x"); { int i; for (i = 0; i < 6; i++) d[i] = 'Q'; } CHECK_EQ_I(tc_p_memcpy_s(d, 6, "12345", n), 0); CHECK_MEM(d, "12QQQQ", 6); }

int main(void) { tc_begin(); TC_LOAD_PROC(memcpy_s); TC_RUN(exported); TC_RUN(exact_fit_returns_0); TC_RUN(dest_larger_than_count); TC_RUN(zero_count_returns_0); TC_RUN(only_count_bytes_written); return tc_end(); }
