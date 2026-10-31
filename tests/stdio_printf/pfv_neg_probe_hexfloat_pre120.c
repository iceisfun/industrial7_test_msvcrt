/* TC
 * id:    pfv_neg_probe_hexfloat_pre120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field; invalid-parameter handler
 * note:  PROBE: unknown conversion 'a'/'A' before VS2013 triggers the invalid-parameter handler: return -1, errno EINVAL, tc_iph_count 1 (msvcr80+ validate the format). Alternative real behaviour (literal output) would be the finding.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static void run(const char *fmt)
{
    char b[64]; volatile double v = 1.0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, fmt, v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(a_invalid_param) { run("%a"); }
TC_CASE(A_invalid_param) { run("%A"); }

int main(void) { tc_begin(); TC_RUN(a_invalid_param); TC_RUN(A_invalid_param); return tc_end(); }
