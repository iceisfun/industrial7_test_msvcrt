/* TC
 * id:    mem_memccpy_first_occurrence
 * area:  memory
 * funcs: _memccpy
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _memccpy
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

TC_CASE(repeated_c_stops_at_first) { static char s[] = "a,b,c,d", d[10]; volatile int c = ','; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 7) == d + 2); CHECK_MEM(d, "a,ZZZZZZZZ", 10); }
TC_CASE(count_zero_returns_null_writes_nothing) { static char s[] = "abc", d[4]; volatile int c = 'a'; volatile size_t n = 0; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, n) == NULL); CHECK_MEM(d, "ZZZZ", 4); }
TC_CASE(c_is_last_byte_of_count) { static char s[] = "abcdef", d[8]; volatile int c = 'c'; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 3) == d + 3); CHECK_MEM(d, "abcZZZZZ", 8); }
TC_CASE(c_at_index_count_not_seen) { static char s[] = "abcdef", d[8]; volatile int c = 'd'; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 3) == NULL); CHECK_MEM(d, "abcZZZZZ", 8); }

int main(void) { tc_begin(); TC_RUN(repeated_c_stops_at_first); TC_RUN(count_zero_returns_null_writes_nothing); TC_RUN(c_is_last_byte_of_count); TC_RUN(c_at_index_count_not_seen); return tc_end(); }
