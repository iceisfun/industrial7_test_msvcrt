/* TC
 * id:    pfm_3_conv_a_len_l_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 a/A (l ignored); MS docs printf type field a
 * note:  explicit .1 precision avoids default-precision differences
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double d = 1.5;
TC_CASE(la) { char b[64]; sprintf(b, "%.1la|%d", d, 7); CHECK_STR(b, "0x1.8p+0|7"); }
TC_CASE(lA) { char b[64]; sprintf(b, "%.1lA|%d", d, 7); CHECK_STR(b, "0X1.8P+0|7"); }
int main(void) { tc_begin(); TC_RUN(la); TC_RUN(lA); return tc_end(); }
