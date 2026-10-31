/* TC
 * id:    mem_memicmp_c_locale_high_bytes
 * area:  memory
 * funcs: _memicmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _memicmp (locale dependent)
 * note:  PROBE: whether _memicmp folds 0xC0/0xE0 in the C locale (expected: no folding, so not equal).
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static int cmp1(unsigned char x, unsigned char y) { volatile unsigned char a[2], b[2]; a[0] = x; b[0] = y; return _memicmp((const void *)a, (const void *)b, 1); }

TC_CASE(0xC0_vs_0xE0_nonzero) { CHECK(cmp1(0xC0, 0xE0) != 0); }
TC_CASE(0xE0_vs_0xE0_zero) { CHECK_EQ_I(cmp1(0xE0, 0xE0), 0); }

int main(void) { tc_begin(); TC_RUN(0xC0_vs_0xE0_nonzero); TC_RUN(0xE0_vs_0xE0_zero); return tc_end(); }
