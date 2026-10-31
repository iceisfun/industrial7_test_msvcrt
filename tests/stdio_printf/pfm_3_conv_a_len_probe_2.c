/* TC
 * id:    pfm_3_conv_a_len_probe_2
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes on a/A
 * note:  PROBING: %.1I32a %.1I64a %.1ja %.1za %.1ta %.1hA format a double like plain %.1a / %.1A (1.5 -> 0x1.8p+0) and consume one double. Explicit precision avoids default-precision uncertainty.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "0x1.8p+0|7");
TC_CASE(I32a) { BODY_T("%.1I32a") }
TC_CASE(I64a) { BODY_T("%.1I64a") }
TC_CASE(ja) { BODY_T("%.1ja") }
TC_CASE(za) { BODY_T("%.1za") }
TC_CASE(ta) { BODY_T("%.1ta") }
TC_CASE(hA) { b[0] = 0; tc_iph_install(); sprintf(b, "%.1hA|%d", d, 7); CHECK_STR(b, "0X1.8P+0|7"); }
int main(void) { tc_begin(); TC_RUN(I32a); TC_RUN(I64a); TC_RUN(ja); TC_RUN(za); TC_RUN(ta); TC_RUN(hA); return tc_end(); }
