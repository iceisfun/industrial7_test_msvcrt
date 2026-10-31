/* TC
 * id:    pfs_probe_str_z_ansi_string
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field: Z = ANSI_STRING/UNICODE_STRING
 * note:  PROBE: %Z with ANSI_STRING pointer prints Length bytes of Buffer (not NUL-bounded). Unverified for UCRT (may be rejected via invalid parameter handler). NULL Buffer expected "(null)" (legacy msvcrt behaviour), kept in its own case.
 */
#include "tc.h"
#include <stdio.h>

typedef struct { unsigned short Length, MaximumLength; char *Buffer; } TC_ANSI_STRING;

TC_CASE(length_bounded) {
  static char buf[] = "abcdef"; TC_ANSI_STRING a; char b[32];
  a.Length = 3; a.MaximumLength = 8; a.Buffer = buf;
  tc_iph_install(); b[0] = 0;
  sprintf(b, "[%Z]", &a);
  CHECK_STR(b, "[abc]");
}
TC_CASE(length_zero) {
  static char buf[] = "abcdef"; TC_ANSI_STRING a; char b[32];
  a.Length = 0; a.MaximumLength = 8; a.Buffer = buf;
  tc_iph_install(); b[0] = 0;
  sprintf(b, "[%Z]", &a);
  CHECK_STR(b, "[]");
}
TC_CASE(null_buffer) {
  TC_ANSI_STRING a; char b[32];
  a.Length = 3; a.MaximumLength = 8; a.Buffer = NULL;
  tc_iph_install(); b[0] = 0;
  sprintf(b, "[%Z]", &a);
  CHECK_STR(b, "[(null)]");
}
int main(void) { tc_begin(); TC_RUN(length_bounded); TC_RUN(length_zero); TC_RUN(null_buffer); return tc_end(); }
