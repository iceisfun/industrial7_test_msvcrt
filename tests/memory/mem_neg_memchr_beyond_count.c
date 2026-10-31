/* TC
 * id:    mem_neg_memchr_beyond_count
 * area:  memory
 * kind:  negative
 * funcs: memchr
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.1: memchr locates the first occurrence in the initial n characters
 * note:  A byte at index >= n must not be found.
 */
#include "tc.h"
#include <string.h>

static const char data[8] = "abcdXXgh";
static volatile size_t v_n;
static volatile int v_c = 'X';

TC_CASE(char_at_index_n_not_found) { v_n = 4; CHECK(memchr(data, v_c, v_n) == NULL); }
TC_CASE(char_after_n_not_found) { v_n = 2; CHECK(memchr(data, v_c, v_n) == NULL); v_n = 3; v_c = 'h'; CHECK(memchr(data, v_c, v_n) == NULL); v_c = 'X'; }
TC_CASE(n_1_char_second_not_found) { v_n = 1; v_c = 'b'; CHECK(memchr(data, v_c, v_n) == NULL); v_c = 'a'; CHECK(memchr(data, v_c, v_n) == data); v_c = 'X'; }
int main(void)
{
    tc_begin();
    TC_RUN(char_at_index_n_not_found); TC_RUN(char_after_n_not_found); TC_RUN(n_1_char_second_not_found);
    return tc_end();
}
