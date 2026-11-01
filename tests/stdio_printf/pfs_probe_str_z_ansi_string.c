/* TC
 * id:    pfs_probe_str_z_ansi_string
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field: Z = ANSI_STRING/UNICODE_STRING
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): %Z with an ANSI_STRING of Length 3 FAILS (return -1, errno EILSEQ, output empty), not "[abc]". Length 0 prints "[]" and a NULL Buffer prints "[(null)]"
 *        (those two cases passed in the baseline and are kept).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef struct { unsigned short Length, MaximumLength; char *Buffer; } TC_ANSI_STRING;

TC_CASE(length_bounded) {
  static char buf[] = "abcdef"; TC_ANSI_STRING a; char b[32]; int r;
  a.Length = 3; a.MaximumLength = 8; a.Buffer = buf;
  tc_iph_install(); b[0] = 0;
  errno = 0; r = sprintf(b, "[%Z]", &a);
  CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_STR(b, "");
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
