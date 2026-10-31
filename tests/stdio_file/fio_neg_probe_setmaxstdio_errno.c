/* TC
 * id:    fio_neg_probe_setmaxstdio_errno
 * area:  stdio_file
 * funcs: _setmaxstdio
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmaxstdio
 * note:  Probes that _setmaxstdio(8193) goes through parameter validation: invalid-parameter handler called once, errno EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(too_big)
{
    volatile int n = 8193;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_setmaxstdio(n), -1);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(too_big); return tc_end(); }
