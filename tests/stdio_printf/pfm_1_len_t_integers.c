/* TC
 * id:    pfm_1_len_t_integers
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1p7 (t = ptrdiff_t); MS docs printf Size prefixes (t supported by UCRT)
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>

static char b[64];
TC_CASE(ti) { volatile ptrdiff_t v = -9; sprintf(b, "%ti", v); CHECK_STR(b, "-9"); }
TC_CASE(tu) { volatile ptrdiff_t v = 9; sprintf(b, "%tu", v); CHECK_STR(b, "9"); }
TC_CASE(tx) { volatile ptrdiff_t v = 255; sprintf(b, "%tx", v); CHECK_STR(b, "ff"); }
TC_CASE(tX) { volatile ptrdiff_t v = 255; sprintf(b, "%tX", v); CHECK_STR(b, "FF"); }

int main(void)
{
    tc_begin();
    TC_RUN(ti); TC_RUN(tu); TC_RUN(tx); TC_RUN(tX);
    return tc_end();
}
