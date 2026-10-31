/* TC
 * id:    pfv_hexfloat_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 a,A conversion specifiers
 * note:  All with explicit precision; default digit count is in a separate probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64];
TC_CASE(one_prec1) { volatile double d = 1.0; sprintf(b, "%.1a", d); CHECK_STR(b, "0x1.0p+0"); }
TC_CASE(one_prec0) { volatile double d = 1.0; sprintf(b, "%.0a", d); CHECK_STR(b, "0x1p+0"); }
TC_CASE(upper_prec2) { volatile double d = 255.0; sprintf(b, "%.2A", d); CHECK_STR(b, "0X1.FEP+7"); }
TC_CASE(half) { volatile double d = 0.5; sprintf(b, "%.1a", d); CHECK_STR(b, "0x1.0p-1"); }
TC_CASE(neg_zero_prec1) { volatile double d = -0.0; sprintf(b, "%.1a", d); CHECK_STR(b, "-0x0.0p+0"); }
TC_CASE(inf) { volatile double d = __builtin_inf(); sprintf(b, "%a", d); CHECK_STR(b, "inf"); }

int main(void) { tc_begin(); TC_RUN(one_prec1); TC_RUN(one_prec0); TC_RUN(upper_prec2); TC_RUN(half); TC_RUN(neg_zero_prec1); TC_RUN(inf); return tc_end(); }
