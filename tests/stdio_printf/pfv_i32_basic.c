/* TC
 * id:    pfv_i32_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf size prefixes I32
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(I32d) { char b[64]; volatile int v = -7; sprintf(b, "%I32d", v); CHECK_STR(b, "-7"); }
TC_CASE(I32u) { char b[64]; volatile unsigned int v = 4294967295u; sprintf(b, "%I32u", v); CHECK_STR(b, "4294967295"); }

int main(void) { tc_begin(); TC_RUN(I32d); TC_RUN(I32u); return tc_end(); }
