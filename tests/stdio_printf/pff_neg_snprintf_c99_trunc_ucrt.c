/* TC
 * id:    pff_neg_snprintf_c99_trunc_ucrt
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf; MS docs snprintf (UCRT is C99 conformant)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = 'Z'; }
TC_CASE(count4) { char b[32]; volatile double v = 1.5; int n; fill(b); n = snprintf(b, 4, "%f", v);
  CHECK_EQ_I(n, 8); CHECK_STR(b, "1.5"); CHECK(b[4] == 'Z'); }
TC_CASE(count1) { char b[32]; volatile double v = 1.5; int n; fill(b); n = snprintf(b, 1, "%f", v);
  CHECK_EQ_I(n, 8); CHECK(b[0] == 0); CHECK(b[1] == 'Z'); }
TC_CASE(count0) { char b[32]; volatile double v = 1.5; int n; fill(b); n = snprintf(b, 0, "%f", v);
  CHECK_EQ_I(n, 8); CHECK(b[0] == 'Z'); }
TC_CASE(null0) { volatile double v = 1.5; char *volatile p = NULL; int n = snprintf(p, 0, "%.3f", v); CHECK_EQ_I(n, 5); }
int main(void) { tc_begin(); TC_RUN(count4); TC_RUN(count1); TC_RUN(count0); TC_RUN(null0); return tc_end(); }
