/* TC
 * id:    pfs_neg_snprintf_truncation
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: more than count chars -> count chars stored, returns negative; exactly count -> no NUL; fewer -> NUL appended
 * note:  does not assert errno or the invalid-parameter handler (not documented for truncation).
 *        count==0 with a non-NULL buffer is deliberately not asserted here; see pfs_neg_probe_snprintf_count0_nonnull.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile v_s = "abcdef";
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(too_small) {
  char b[16]; int n; fill(b);
  n = _snprintf(b, 4, "%s", v_s);
  CHECK_EQ_I(n, -1);
  CHECK(b[0]=='a' && b[1]=='b' && b[2]=='c' && b[3]=='d'); CHECK(b[4] == 'Z');
}
TC_CASE(count_equals_len) {
  char b[16]; int n; fill(b);
  n = _snprintf(b, 3, "abc");
  CHECK_EQ_I(n, 3);
  CHECK(b[0]=='a' && b[1]=='b' && b[2]=='c'); CHECK(b[3] == 'Z');
}
TC_CASE(precision_fits) {
  char b[16]; int n; fill(b);
  n = _snprintf(b, 4, "%.3s", v_s);
  CHECK_EQ_I(n, 3); CHECK_STR(b, "abc");
}

int main(void) { tc_begin(); TC_RUN(too_small); TC_RUN(count_equals_len); TC_RUN(precision_fits); return tc_end(); }
