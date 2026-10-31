/* TC
 * id:    pfv_probe_null_string_precision
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none (undefined in ISO C)
 * note:  PROBE: precision applies to the substituted "(null)" text, so %.3s of NULL gives "(nu".
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static char b[64];
static const char *volatile np;
TC_CASE(precision_trunc) { sprintf(b, "[%.3s]", np); CHECK_STR(b, "[(nu]"); }

int main(void) { tc_begin(); TC_RUN(precision_trunc); return tc_end(); }
