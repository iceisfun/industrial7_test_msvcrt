/* TC
 * id:    pfm_2_conv_z_prec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field Z (ANSI_STRING)
 * note:  PROBE: precision is NOT applied to %Z (the CRT prints Length bytes): "%.1Z" of "ab" -> "ab". Alternative: "a".
 */
#include "tc.h"
#include <stdio.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
static char buf[8] = { 'a','b','c','d','X','X','X','X' };
static TC_ANSI a = { 2, 8, buf };

TC_CASE(prec)   { char b[32]; TC_ANSI * volatile pa = &a; sprintf(b, "%.1Z", pa);   CHECK_STR(b, "ab"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(prec);
    return tc_end();
}
