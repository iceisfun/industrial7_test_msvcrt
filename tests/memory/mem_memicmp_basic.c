/* TC
 * id:    mem_memicmp_basic
 * area:  memory
 * funcs: _memicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _memicmp, _memicmp_l
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

TC_CASE(equal_ignoring_case_returns_0) { static char a[] = "Hello, World", b[] = "hELLO, wORLD"; volatile size_t n = 12; CHECK_EQ_I(_memicmp(a, b, n), 0); }
TC_CASE(less) { static char a[] = "abcd", b[] = "abce"; volatile size_t n = 4; CHECK(_memicmp(a, b, n) < 0); static char c[] = "ABCD"; CHECK(_memicmp(c, b, n) < 0); }
TC_CASE(greater) { static char a[] = "abcf", b[] = "ABCE"; volatile size_t n = 4; CHECK(_memicmp(a, b, n) > 0); }
TC_CASE(count_limits_compare) { static char a[] = "abcX", b[] = "ABCy"; volatile size_t n = 3; CHECK_EQ_I(_memicmp(a, b, n), 0); n = 4; CHECK(_memicmp(a, b, n) < 0); }
TC_CASE(zero_count_returns_0) { static char a[] = "a", b[] = "z"; volatile size_t n = 0; CHECK_EQ_I(_memicmp(a, b, n), 0); }

int main(void) { tc_begin(); TC_RUN(equal_ignoring_case_returns_0); TC_RUN(less); TC_RUN(greater); TC_RUN(count_limits_compare); TC_RUN(zero_count_returns_0); return tc_end(); }
