/* TC
 * id:    pfm_3_conv_cap_f_len_l_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 F; MS docs printf type field F
 * note:  %F is documented for the UCRT; l is ignored for floating conversions
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double d = 1.25;
TC_CASE(lF) { char b[64]; sprintf(b, "%lF|%d", d, 7); CHECK_STR(b, "1.250000|7"); }
int main(void) { tc_begin(); TC_RUN(lF); return tc_end(); }
