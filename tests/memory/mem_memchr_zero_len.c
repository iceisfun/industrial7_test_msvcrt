/* TC
 * id:    mem_memchr_zero_len
 * area:  memory
 * funcs: memchr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.5.1; MS docs memchr
 */
#include "tc.h"
#include <string.h>

TC_CASE(zero_len_returns_null) { static char b[4] = "abc"; volatile size_t n = 0; CHECK(memchr(b, 'b', n) == NULL); CHECK(memchr(b, 'z', n) == NULL); }
TC_CASE(zero_len_char_present_at_0) { static char b[4] = "abc"; volatile size_t n = 0; volatile int c = 'a'; CHECK(memchr(b, c, n) == NULL); CHECK(memchr(b, c, 1) == b); }

int main(void) { tc_begin(); TC_RUN(zero_len_returns_null); TC_RUN(zero_len_char_present_at_0); return tc_end(); }
