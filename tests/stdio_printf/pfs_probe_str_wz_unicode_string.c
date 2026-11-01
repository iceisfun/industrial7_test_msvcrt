/* TC
 * id:    pfs_probe_str_wz_unicode_string
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf type field: Z / wZ (UNICODE_STRING)
 * note:  PROBE: %wZ takes a UNICODE_STRING pointer, Length is in BYTES. Win10 22H2 observed: the first two cases pass as written. The old last case (plain %Z with a UNICODE_STRING in wide printf) was WRONG: in wide printf a plain %Z takes an ANSI_STRING (narrow chars) - the UNICODE_STRING form is %wZ - so it now has two cases, %wZ+UNICODE_STRING and %Z+ANSI_STRING, both giving "[abc]" (5). Raw: Z_in_wide wide string mismatch b == L"[abc]" (printf returned -1 after "[a").
 
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

typedef struct { unsigned short Length, MaximumLength; wchar_t *Buffer; } TC_UNICODE_STRING;
typedef struct { unsigned short Length, MaximumLength; char *Buffer; } TC_ANSI_STRING;
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
TC_CASE(wZ_in_wide) {
  TC_UNICODE_STRING u; wchar_t b[32]; int r;
  u.Length = 3 * sizeof(wchar_t); u.MaximumLength = 14; u.Buffer = wbuf;
  tc_iph_install(); b[0] = 0;
  r = _snwprintf(b, 32, L"[%wZ]", &u);
  CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"[abc]");
}
TC_CASE(Z_ansi_in_wide) {   /* observed: in the WIDE printf a plain %Z takes an ANSI_STRING (narrow chars), not a UNICODE_STRING */
  TC_ANSI_STRING a; wchar_t b[32]; int r; static char txt[] = "abc";
  a.Length = 3; a.MaximumLength = 4; a.Buffer = txt;
  tc_iph_install(); b[0] = 0;
  r = _snwprintf(b, 32, L"[%Z]", &a);
  CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"[abc]");
}
int main(void) { tc_begin(); TC_RUN(wZ_len_bytes); TC_RUN(wZ_zero); TC_RUN(wZ_in_wide); TC_RUN(Z_ansi_in_wide); return tc_end(); }
