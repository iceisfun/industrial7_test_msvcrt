/* TC
 * id:    pfv_neg_probe_hh_pre120
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh absent before VS2013)
 * note:  PROBE: sprintf("%hhd",1) pre-VS2013: second 'h' is an unknown conversion -> handler called, -1, EINVAL. If instead it is treated as 'h' that is the finding.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(hhd_invalid)
{
    char b[64]; int n; volatile int v = 1;
    b[0] = 'x'; b[1] = 0; tc_iph_count = 0; errno = 0;
    n = sprintf(b, "%hhd", v);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(hhd_invalid); return tc_end(); }
