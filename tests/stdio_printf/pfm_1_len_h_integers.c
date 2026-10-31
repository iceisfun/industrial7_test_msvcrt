/* TC
 * id:    pfm_1_len_h_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7 (h = short / unsigned short); MS docs printf size prefixes
 */
#include "tc.h"
#include <stdio.h>

static char b[64];
TC_CASE(hi_wrap)   { volatile int v = 65537; sprintf(b, "%hi", v); CHECK_STR(b, "1"); }
TC_CASE(hi_neg)    { volatile int v = 40000; sprintf(b, "%hi", v); CHECK_STR(b, "-25536"); }
TC_CASE(hX)        { volatile int v = 0x1ABCD; sprintf(b, "%hX", v); CHECK_STR(b, "ABCD"); }
TC_CASE(ho)        { volatile int v = 65536; sprintf(b, "%ho", v); CHECK_STR(b, "0"); }
TC_CASE(hu_neg)    { volatile int v = -1; sprintf(b, "%hu", v); CHECK_STR(b, "65535"); }
TC_CASE(hd_width)  { volatile int v = 70000; sprintf(b, "[%8hd]", v); CHECK_STR(b, "[    4464]"); }

int main(void)
{
    tc_begin();
    TC_RUN(hi_wrap); TC_RUN(hi_neg); TC_RUN(hX); TC_RUN(ho); TC_RUN(hu_neg); TC_RUN(hd_width);
    return tc_end();
}
