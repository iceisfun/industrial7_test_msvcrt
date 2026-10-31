/* TC
 * id:    pfv_neg_probe_zjt_pre120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf size prefixes; invalid-parameter handler
 * note:  PROBE: 'z','j','t' are not size prefixes before VS2013 -> unknown conversion: handler called, return -1, errno EINVAL. Alternative real behaviour (literal output) would be the finding.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static void run(const char *fmt)
{
    char b[64]; int r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, fmt, 5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zu_invalid) { run("%zu"); }
TC_CASE(jd_invalid) { run("%jd"); }
TC_CASE(td_invalid) { run("%td"); }

int main(void) { tc_begin(); TC_RUN(zu_invalid); TC_RUN(jd_invalid); TC_RUN(td_invalid); return tc_end(); }
