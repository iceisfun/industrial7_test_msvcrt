/* TC
 * id:    pfi_probe_w_length_int
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf width/size specification (w defined for c/s)
 * note:  PROBE: MS docs only define w for c/s; likely accepted and ignored for integers (iph count 0, output 5).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f = "%wd";
TC_CASE(wd) { char b[64]; int n; tc_iph_install(); tc_iph_count = 0; b[0] = 0; n = sprintf(b, f, 5); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "5"); CHECK_EQ_I(n, 1); }
int main(void) { tc_begin(); TC_RUN(wd); return tc_end(); }
