/* TC
 * id:    pfs_neg_sprintf_null_buffer
 * area:  stdio_printf
 * funcs: sprintf _snprintf _snwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf, _snprintf (Parameter validation: NULL buffer with nonzero count -> invalid parameter handler, -1, EINVAL)
 * note:  count 0 with a NULL buffer is deliberately not tested here (see pfs_probe_vsnprintf_null_count0).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char *volatile v_nullb = NULL;
static wchar_t *volatile v_nullw = NULL;

TC_CASE(sprintf_null) {
  int n; tc_iph_count = 0; errno = 0;
  n = sprintf(v_nullb, "%s", "x");
  CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snprintf_null) {
  int n; tc_iph_count = 0; errno = 0;
  n = _snprintf(v_nullb, 5, "x");
  CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snwprintf_null) {
  int n; tc_iph_count = 0; errno = 0;
  n = _snwprintf(v_nullw, 5, L"x");
  CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(sprintf_null); TC_RUN(snprintf_null); TC_RUN(snwprintf_null); return tc_end(); }
