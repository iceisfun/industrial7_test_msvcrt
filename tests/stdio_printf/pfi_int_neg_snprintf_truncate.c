/* TC
 * id:    pfi_int_neg_snprintf_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns -1 if more than count chars needed; no NUL if exactly count
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
static void memset_(char *b) { int i; for (i = 0; i < 16; i++) b[i] = (char)90; }

TC_CASE(overflow) {
  char b[16]; int n; memset_(b);
  n = _snprintf(b, 3, "%d", 12345);
  CHECK_EQ_I(n, -1);
  CHECK(b[0]=='1' && b[1]=='2' && b[2]=='3'); CHECK(b[3]=='Z');
}
TC_CASE(exact) {
  char b[16]; int n; memset_(b);
  n = _snprintf(b, 5, "%d", 12345);
  CHECK_EQ_I(n, 5);
  CHECK(b[0]=='1' && b[4]=='5'); CHECK(b[5]=='Z');
}
TC_CASE(fits) {
  char b[16]; int n; memset_(b);
  n = _snprintf(b, 6, "%d", 12345);
  CHECK_EQ_I(n, 5); CHECK_STR(b, "12345");
}
TC_CASE(neg_sign_counts) {
  char b[16]; int n; memset_(b);
  n = _snprintf(b, 5, "%d", -12345);
  CHECK_EQ_I(n, -1); CHECK(b[4]=='4'); CHECK(b[5]=='Z');
}

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(exact); TC_RUN(fits); TC_RUN(neg_sign_counts); return tc_end(); }
