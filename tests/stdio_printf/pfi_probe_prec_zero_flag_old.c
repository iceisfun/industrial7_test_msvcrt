/* TC
 * id:    pfi_probe_prec_zero_flag_old
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 p6 (0 flag ignored with precision)
 * note:  PROBE: precision overrides 0 flag and %.0d of 0 is empty on Win7-era CRTs (as standard).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f1 = "[%08.3d]"; static char *volatile f2 = "[%.0d]"; static char *volatile f3 = "[%#.0o]";
TC_CASE(prec_over_zero) { char b[64]; sprintf(b, f1, 7); CHECK_STR(b, "[     007]"); }
TC_CASE(prec0_zero) { char b[64]; sprintf(b, f2, 0); CHECK_STR(b, "[]"); }
TC_CASE(alt_octal_prec0) { char b[64]; sprintf(b, f3, 0); CHECK_STR(b, "[0]"); }
int main(void) { tc_begin(); TC_RUN(prec_over_zero); TC_RUN(prec0_zero); TC_RUN(alt_octal_prec0); return tc_end(); }
