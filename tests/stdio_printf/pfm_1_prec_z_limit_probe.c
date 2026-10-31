/* TC
 * id:    pfm_1_prec_z_limit_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field Z (ANSI_STRING / UNICODE_STRING)
 * note:  PROBE: whether a precision limits %Z output (%.2Z of ANSI_STRING "abcd" -> "ab"). The docs say nothing about precision for Z; a legacy CRT may print all Length bytes (abcd) instead. Isolated guess.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;

TC_CASE(Z_prec) {
    char b[32]; char buf[8] = { 'a','b','c','d','X','X','X','X' }; TC_ANSI a; TC_ANSI * volatile pa = &a;
    a.Length = 4; a.MaximumLength = 8; a.Buffer = buf;
    tc_iph_install();
    sprintf(b, "[%.2Z]", pa); CHECK_STR(b, "[ab]");
}

int main(void) { tc_begin(); TC_RUN(Z_prec); return tc_end(); }
