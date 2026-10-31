/* TC
 * id:    pfm_1_neg_z_ucrt_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field (ucrt does not list Z)
 * note:  PROBE: ucrt does not support the %Z conversion; it is rejected as an invalid format: return -1, errno EINVAL, invalid-parameter handler called once.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(Z_rejected) {
    char b[32]; volatile int dummy = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, "%Z", (void *)&dummy);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(Z_rejected); return tc_end(); }
