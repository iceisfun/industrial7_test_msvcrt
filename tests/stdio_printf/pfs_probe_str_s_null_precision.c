/* TC
 * id:    pfs_probe_str_s_null_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf
 * note:  PROBE: whether precision applies to the substituted (null) literal. Expected most likely: %.3s -> (nu, %.0s -> empty.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile np = NULL;
TC_CASE(prec3) { char b[64]; sprintf(b, "[%.3s]", np); CHECK_STR(b, "[(nu]"); }
TC_CASE(prec0) { char b[64]; sprintf(b, "[%.0s]", np); CHECK_STR(b, "[]"); }
int main(void) { tc_begin(); TC_RUN(prec3); TC_RUN(prec0); return tc_end(); }
