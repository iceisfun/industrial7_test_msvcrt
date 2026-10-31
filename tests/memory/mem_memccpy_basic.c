/* TC
 * id:    mem_memccpy_basic
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

TC_CASE(found_returns_dest_plus_idx_plus_1) { static char s[] = "hello world", d[16]; volatile int c = ' '; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 11) == d + 6); CHECK_MEM(d, "hello ", 6); }
TC_CASE(bytes_after_c_not_written) { static char s[] = "abcdef", d[8]; volatile int c = 'c'; memset(d, 'Z', sizeof d); _memccpy(d, s, c, 6); CHECK_MEM(d, "abcZZZZZ", 8); }
TC_CASE(not_found_returns_null_and_copies_count) { static char s[] = "abcdef", d[8]; volatile int c = 'x'; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 4) == NULL); CHECK_MEM(d, "abcdZZZZ", 8); }
TC_CASE(c_in_first_byte) { static char s[] = "abcdef", d[4]; volatile int c = 'a'; memset(d, 'Z', sizeof d); CHECK(_memccpy(d, s, c, 6) == d + 1); CHECK_MEM(d, "aZZZ", 4); }

int main(void) { tc_begin(); TC_RUN(found_returns_dest_plus_idx_plus_1); TC_RUN(bytes_after_c_not_written); TC_RUN(not_found_returns_null_and_copies_count); TC_RUN(c_in_first_byte); return tc_end(); }
