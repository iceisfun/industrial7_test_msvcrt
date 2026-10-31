/* TC
 * id:    pfs_neg_sprintf_null_format
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf (Parameter validation: format NULL invokes the invalid parameter handler; returns -1, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile v_null_fmt = NULL;

TC_CASE(null_format) {
  char b[16]; int n, i;
  for (i = 0; i < 16; i++) b[i] = 'Z';
  tc_iph_count = 0; errno = 0;
  n = sprintf(b, v_null_fmt);
  CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  for (i = 1; i < 16; i++) CHECK(b[i] == 'Z');
}
TC_CASE(works_after_failure) {
  char b[16]; int n;
  tc_iph_count = 0;
  n = sprintf(b, "%s", "ok");
  CHECK_EQ_I(n, 2); CHECK_STR(b, "ok"); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_format); TC_RUN(works_after_failure); return tc_end(); }
