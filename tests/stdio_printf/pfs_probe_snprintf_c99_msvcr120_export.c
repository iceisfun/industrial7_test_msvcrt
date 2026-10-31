/* TC
 * id:    pfs_probe_snprintf_c99_msvcr120_export
 * area:  stdio_printf
 * funcs:
 * procs: snprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs snprintf (VS2013+ C99 conforming snprintf)
 * note:  PROBE: whether msvcr120.dll exports `snprintf` and whether it follows C99 (returns would-be length, always NUL terminates). Skips if not exported.
 */
#include "tc.h"
#include <string.h>

typedef int (__cdecl *snprintf_t)(char *, size_t, const char *, ...);
static snprintf_t p_snprintf;

TC_CASE(c99_semantics) {
  char b[8]; int n; memset(b, 'Z', sizeof b);
  if (!p_snprintf) TC_SKIP("snprintf not exported");
  n = p_snprintf(b, 3, "%s", "abcdef");
  CHECK_EQ_I(n, 6); CHECK_STR(b, "ab");
}
TC_CASE(null_size0) {
  if (!p_snprintf) TC_SKIP("snprintf not exported");
  CHECK_EQ_I(p_snprintf(NULL, 0, "%s", "hello"), 5);
}
int main(void) { tc_begin(); p_snprintf = (snprintf_t)tc_crt_proc("snprintf"); TC_RUN(c99_semantics); TC_RUN(null_size0); return tc_end(); }
