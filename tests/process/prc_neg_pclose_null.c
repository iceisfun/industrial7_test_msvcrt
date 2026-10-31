/* TC
 * id:    prc_neg_pclose_null
 * area:  process
 * funcs: _pclose
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _pclose (returns -1 on error)
 * note:  NULL stream -> iph + -1 + EINVAL is CRT parameter validation (>=80); docs do not spell it out; hence inferred.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static FILE *volatile vf;
TC_CASE(null_stream)
{
    int r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0; vf = NULL;
    r = _pclose(vf);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_stream); return tc_end(); }
