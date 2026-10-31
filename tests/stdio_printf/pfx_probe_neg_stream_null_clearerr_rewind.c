/* TC
 * id:    pfx_probe_neg_stream_null_clearerr_rewind
 * area:  stdio_printf
 * kind:  negative
 * funcs: clearerr rewind
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs clearerr, rewind
 * note:  PROBE: clearerr(NULL) and rewind(NULL) each invoke the invalid parameter handler exactly once and return.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nf = NULL;

TC_CASE(clearerr_null) { tc_iph_count = 0; clearerr(nf); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(rewind_null)   { tc_iph_count = 0; rewind(nf); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(clearerr_null); TC_RUN(rewind_null);
    return tc_end();
}
