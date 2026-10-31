/* TC
 * id:    pfm_1_prec_z_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field Z (ANSI_STRING / UNICODE_STRING)
 * note:  PROBE: legacy CRTs support %Z with an ANSI_STRING* argument {USHORT Length, MaximumLength; PCHAR Buffer}; it prints Length bytes of Buffer (no NUL needed). Not asserted for ucrt (see pfm_1_neg_z_ucrt_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;

TC_CASE(Z_plain) {
    char b[32]; char buf[8] = { 'a','b','c','d','X','X','X','X' }; TC_ANSI a; TC_ANSI * volatile pa = &a;
    a.Length = 4; a.MaximumLength = 8; a.Buffer = buf;
    tc_iph_install();
    sprintf(b, "[%Z]", pa); CHECK_STR(b, "[abcd]");
}
int main(void) { tc_begin(); TC_RUN(Z_plain); return tc_end(); }
