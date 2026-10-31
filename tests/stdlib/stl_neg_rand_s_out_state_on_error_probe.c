/* TC
 * id:    stl_neg_rand_s_out_state_on_error_probe
 * area:  stdlib
 * kind:  negative
 * funcs: rand_s
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs rand_s (NULL -> EINVAL via invalid parameter handler; success returns 0)
 * note:  PROBES that a failing rand_s(NULL) does not poison later calls and that a successful rand_s leaves errno unchanged.
 *        The failing-call behaviour (EINVAL return, errno, iph) is documented; the errno-untouched-on-success claim is the guess.
 */
#define _CRT_RAND_S
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_then_valid)
{
    unsigned int *volatile p = NULL; unsigned int v = 0x5a5a5a5a; errno_t r;
    tc_iph_count = 0; errno = 0;
    r = rand_s(p);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = rand_s(&v);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(success_keeps_errno)
{
    unsigned int v; errno_t r; int i;
    for (i = 0; i < 4; i++) { errno = 0; r = rand_s(&v); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); }
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(null_then_valid); TC_RUN(success_keeps_errno);
    return tc_end();
}
