/* TC
 * id:    pfm_3_conv_e_len_l_standard
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p7 (l has no effect on e/f/g/a); MS docs printf size prefixes
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double d = 1.5;
static char b[64];
TC_CASE(le)
{
    sprintf(b, "%.2le|%d", d, 7); 
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.50e+00|7");
#else
    CHECK_STR(b, "1.50e+000|7");
#endif
}
TC_CASE(lf_lg) { sprintf(b, "%lf|%lg|%d", d, d, 7); CHECK_STR(b, "1.500000|1.5|7"); }
int main(void) { tc_begin(); TC_RUN(le); TC_RUN(lf_lg); return tc_end(); }
