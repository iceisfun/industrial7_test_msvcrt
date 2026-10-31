/* TC
 * id:    pfm_2_conv_z_width_prec_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field Z (ANSI_STRING)
 * note:  PROBE: width applies to %Z like to %s, measured against Length ('%5Z' of "ab" -> "   ab", '%-5Z|' -> "ab   |").
 */
#include "tc.h"
#include <stdio.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
static char buf[8] = { 'a','b','c','d','X','X','X','X' };
static TC_ANSI a = { 2, 8, buf };

TC_CASE(width)  { char b[32]; TC_ANSI * volatile pa = &a; sprintf(b, "%5Z", pa);   CHECK_STR(b, "   ab"); }
TC_CASE(left)   { char b[32]; TC_ANSI * volatile pa = &a; sprintf(b, "%-5Z|", pa); CHECK_STR(b, "ab   |"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(width); TC_RUN(left);
    return tc_end();
}
