/* TC
 * id:    pfs_probe_wstr_hs_high_c_locale
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field characters (hs / hc in wide functions); C locale mbtowc
 * note:  PROBE: narrow bytes >127 widened by %hs/%hc in C locale. Expected 1:1 mapping (0xE9 -> U+00E9). Unverified on every CRT.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(hs_high) {
  wchar_t b[16]; volatile const char *s = "\xe9"; int n;
  n = _snwprintf(b, 16, L"%hs", (const char *)s);
  CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 0x00e9); CHECK_EQ_I(b[1], 0);
}
TC_CASE(hc_high) {
  wchar_t b[16]; volatile char c = (char)0xE9; int n;
  n = _snwprintf(b, 16, L"%hc", (int)c);
  CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 0x00e9);
}
int main(void) { tc_begin(); TC_RUN(hs_high); TC_RUN(hc_high); return tc_end(); }
