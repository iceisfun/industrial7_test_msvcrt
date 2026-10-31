/* TC
 * id:    mem_memicmp_lowercase_fold
 * area:  memory
 * funcs: _memicmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _memicmp (case-insensitive); lowercase folding is the long-standing CRT behaviour, not stated in the docs
 * note:  Characters between 'Z' and 'a' sort above upper-case letters only if folded to upper; _memicmp folds to lower.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static int cmp1(unsigned char x, unsigned char y) { volatile unsigned char a[2], b[2]; a[0] = x; b[0] = y; return _memicmp((const void *)a, (const void *)b, 1); }

TC_CASE(underscore_vs_a_negative) { CHECK(cmp1('_', 'a') < 0); CHECK(cmp1('a', '_') > 0); }
TC_CASE(underscore_vs_A_negative) { CHECK(cmp1('_', 'A') < 0); CHECK(cmp1('A', '_') > 0); }
TC_CASE(bracket_vs_a_negative) { CHECK(cmp1('[', 'a') < 0); CHECK(cmp1('[', 'A') < 0); }
TC_CASE(at_vs_A_equal_not) { CHECK(cmp1('@', 'A') != 0); CHECK(cmp1('@', 'A') < 0); CHECK(cmp1('@', '`') != 0); }

int main(void) { tc_begin(); TC_RUN(underscore_vs_a_negative); TC_RUN(underscore_vs_A_negative); TC_RUN(bracket_vs_a_negative); TC_RUN(at_vs_A_equal_not); return tc_end(); }
