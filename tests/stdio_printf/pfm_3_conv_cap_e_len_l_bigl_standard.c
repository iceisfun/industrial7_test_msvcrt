/* TC
 * id:    pfm_3_conv_cap_e_len_l_bigl_standard
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.21.6.1 (l ignored for E); MS docs printf size prefixes (L = long double == double)
 * note:  %LE passes a double: MS documents long double as identical to double, so this is not UB on Windows (it would be UB on other platforms)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#if TC_AT_LEAST(140)
#define W "1.500000E+00|7"
#else
#define W "1.500000E+000|7"
#endif
TC_CASE(lE) { sprintf(b, "%lE|%d", d, 7); CHECK_STR(b, W); }
TC_CASE(LE) { sprintf(b, "%LE|%d", d, 7); CHECK_STR(b, W); }
int main(void) { tc_begin(); TC_RUN(lE); TC_RUN(LE); return tc_end(); }
