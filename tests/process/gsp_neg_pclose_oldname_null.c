/* TC
 * id:    gsp_neg_pclose_oldname_null
 * area:  process
 * funcs: _pclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _pclose (parameter validation: NULL stream -> invalid parameter handler, EINVAL, returns -1)
 * note:  Unprefixed pclose. Non-pipe FILE is covered by prc_neg_pclose_non_pipe_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile g_nul = NULL;

TC_CASE(null_stream)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = pclose(g_nul);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_stream); return tc_end(); }
