/* TC
 * id:    pfi_probe_star_intmin_width
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf width specification
 * note:  PROBE: %*d with width INT_MIN (negation overflows). Records either -1+iph or a huge output; asserts only that the call returns and iph/ret are consistent.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f = "%*d";
TC_CASE(intmin) {
  int n; tc_iph_install(); tc_iph_count = 0;
  n = _scprintf(f, INT_MIN, 1);
  if (tc_iph_count > 0) CHECK_EQ_I(n, -1);
}
int main(void) { tc_begin(); TC_RUN(intmin); return tc_end(); }
