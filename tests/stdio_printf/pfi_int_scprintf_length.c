/* TC
 * id:    pfi_int_scprintf_length
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf: returns number of characters that would be written, excluding the terminating NUL
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(intmin) { volatile int v = INT_MIN; CHECK_EQ_I(_scprintf("%d", v), 11); }
TC_CASE(prec_zero) { volatile int z = 0; CHECK_EQ_I(_scprintf("%.0d", z), 0); CHECK_EQ_I(_scprintf("[%.0d]", z), 2); }
TC_CASE(width) { CHECK_EQ_I(_scprintf("%10d", 5), 10); CHECK_EQ_I(_scprintf("%-10d|", 5), 11); }
TC_CASE(u64) { volatile unsigned long long v = ULLONG_MAX; CHECK_EQ_I(_scprintf("%I64u", v), 20); }
TC_CASE(star) { CHECK_EQ_I(_scprintf("%*d", 8, 1), 8); CHECK_EQ_I(_scprintf("%.*d", 6, 1), 6); }
TC_CASE(alt) { CHECK_EQ_I(_scprintf("%#x", 255), 4); CHECK_EQ_I(_scprintf("%#o", 8), 3); CHECK_EQ_I(_scprintf("%#x", 0), 1); }
int main(void) { tc_begin(); TC_RUN(intmin); TC_RUN(prec_zero); TC_RUN(width); TC_RUN(u64); TC_RUN(star); TC_RUN(alt); return tc_end(); }
