/* TC
 * id:    mem_memicmp_return_value_probe
 * area:  memory
 * funcs: _memicmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _memicmp
 * note:  PROBE: whether _memicmp returns the lowercased byte difference (-4/+4) rather than -1/+1 for less/greater.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static int cmp1(unsigned char x, unsigned char y) { volatile unsigned char a[2], b[2]; a[0] = x; b[0] = y; return _memicmp((const void *)a, (const void *)b, 1); }

TC_CASE(less_is_minus_4) { CHECK_EQ_I(cmp1('a', 'e'), -4); }
TC_CASE(greater_is_plus_4) { CHECK_EQ_I(cmp1('e', 'a'), 4); }

int main(void) { tc_begin(); TC_RUN(less_is_minus_4); TC_RUN(greater_is_plus_4); return tc_end(); }
