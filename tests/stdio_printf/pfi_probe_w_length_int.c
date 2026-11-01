/* TC
 * id:    pfi_probe_w_length_int
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf width/size specification (w defined for c/s)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): MS docs only define w for c/s; for integers "%wd" is an invalid format: handler once, -1, errno EINVAL, empty output
 *        (old guess "accepted and ignored" was wrong).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
static char *volatile f = "%wd";
TC_CASE(wd) { char b[64]; int n; tc_iph_install(); tc_iph_count = 0; errno = 0; b[0] = 0; n = sprintf(b, f, 5); CHECK_EQ_I(tc_iph_count, 1); CHECK_STR(b, ""); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); }
int main(void) { tc_begin(); TC_RUN(wd); return tc_end(); }
