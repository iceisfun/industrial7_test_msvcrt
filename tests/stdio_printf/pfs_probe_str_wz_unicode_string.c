/* TC
 * id:    pfs_probe_str_wz_unicode_string
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field: Z / wZ (UNICODE_STRING)
 * note:  PROBE: %wZ takes a UNICODE_STRING pointer, Length is in BYTES. Last case: %Z with a UNICODE_STRING in wide printf. All unverified for UCRT.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

typedef struct { unsigned short Length, MaximumLength; wchar_t *Buffer; } TC_UNICODE_STRING;
static wchar_t wbuf[] = L"abcdef";

TC_CASE(wZ_len_bytes) {
  TC_UNICODE_STRING u; char b[32];
  u.Length = 3 * sizeof(wchar_t); u.MaximumLength = 14; u.Buffer = wbuf;
  tc_iph_install(); b[0] = 0;
  sprintf(b, "[%wZ]", &u);
  CHECK_STR(b, "[abc]");
}
TC_CASE(wZ_zero) {
  TC_UNICODE_STRING u; char b[32];
  u.Length = 0; u.MaximumLength = 14; u.Buffer = wbuf;
  tc_iph_install(); b[0] = 0;
  sprintf(b, "[%wZ]", &u);
  CHECK_STR(b, "[]");
}
TC_CASE(Z_in_wide) {
  TC_UNICODE_STRING u; wchar_t b[32];
  u.Length = 3 * sizeof(wchar_t); u.MaximumLength = 14; u.Buffer = wbuf;
  tc_iph_install(); b[0] = 0;
  _snwprintf(b, 32, L"[%Z]", &u);
  CHECK_WSTR(b, L"[abc]");
}
int main(void) { tc_begin(); TC_RUN(wZ_len_bytes); TC_RUN(wZ_zero); TC_RUN(Z_in_wide); return tc_end(); }
