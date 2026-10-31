/* TC
 * id:    pff_neg__snprintf_f_trunc
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: if more than count chars needed returns negative and the buffer is not NUL-terminated; exactly count -> count, no NUL
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = 'Z'; }
TC_CASE(count4) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 4, "%f", v);
  CHECK_EQ_I(n, -1); CHECK(memcmp(b, "1.50", 4) == 0); CHECK(b[4] == 'Z'); }
TC_CASE(count7) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 7, "%f", v);
  CHECK_EQ_I(n, -1); CHECK(memcmp(b, "1.50000", 7) == 0); CHECK(b[7] == 'Z'); }
TC_CASE(count8_exact) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 8, "%f", v);
  CHECK_EQ_I(n, 8); CHECK(memcmp(b, "1.500000", 8) == 0); CHECK(b[8] == 'Z'); }
TC_CASE(count9_fits) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 9, "%f", v);
  CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); }
int main(void) { tc_begin(); TC_RUN(count4); TC_RUN(count7); TC_RUN(count8_exact); TC_RUN(count9_fits); return tc_end(); }
