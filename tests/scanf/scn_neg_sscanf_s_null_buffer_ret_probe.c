/* TC
 * id:    scn_neg_sscanf_s_null_buffer_ret_probe
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs scanf_s (invalid parameter handler); return value/errno not documented precisely
 * note:  PROBES: after sscanf_s("5 abc","%d%s",&i,NULL,8) with a returning handler, the return value is 1 (count assigned
 *        so far; could instead be EOF), errno == EINVAL, and the int assigned before the failure keeps its value 5.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char *volatile vdst;
TC_CASE(ret_errno)
{
    int i = 0, n; vdst = NULL; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = sscanf_s("5 abc", "%d%s", &i, vdst, 8u);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(i, 5);
}
int main(void) { tc_begin(); TC_RUN(ret_errno); return tc_end(); }
