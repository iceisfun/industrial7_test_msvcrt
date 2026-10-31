/* TC
 * id:    pff_neg_star_precision_int_min
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 p4 (a negative precision argument is taken as if the precision were omitted)
 * note:  INT_MIN is the extreme negative precision; the output must be the default-precision text and fit in the buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(f)  { char b[64]; volatile int p = INT_MIN; int r = sprintf(b, "%.*f", p, 1.5); CHECK_STR(b, "1.500000"); CHECK_EQ_I(r, 8); }
TC_CASE(g)  { char b[64]; volatile int p = INT_MIN; int r = sprintf(b, "%.*g", p, 1.5); CHECK_STR(b, "1.5"); CHECK_EQ_I(r, 3); }

int main(void) { tc_begin(); TC_RUN(f); TC_RUN(g); return tc_end(); }
