/* TC
 * id:    stl_neg_swab_zero_count_null_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _swab "Parameter validation"
 * note:  PROBES whether _swab validates NULL pointers when n == 0. Expectation: the UCRT validates pointers only when n > 0
 *        (n == 0 with NULL: no iph); n > 0 with NULL: iph once, errno EINVAL. Real Windows is ground truth.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TC_CASE(zero_valid_control)
{
    char s[2] = { 'a', 'b' }, d[2] = { 'x', 'y' };
    tc_iph_count = 0; errno = 0; _swab(s, d, 0);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(d[0], 'x'); CHECK_EQ_I(d[1], 'y');
}
TC_CASE(zero_null_src)  { char d[2] = { 'x', 'y' }; char *volatile p = NULL; tc_iph_count = 0; errno = 0; _swab(p, d, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(d[0], 'x'); }
TC_CASE(zero_null_dest) { char s[2] = { 'a', 'b' }; char *volatile p = NULL; tc_iph_count = 0; errno = 0; _swab(s, p, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(zero_both_null) { char *volatile p = NULL; tc_iph_count = 0; errno = 0; _swab(p, p, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(two_null_combos)
{
    char s[2] = { 'a', 'b' }, d[2] = { 'x', 'y' }; char *volatile p = NULL;
    tc_iph_count = 0; errno = 0; _swab(p, d, 2); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(d[0], 'x');
    errno = 0; _swab(s, p, 2); CHECK_EQ_I(tc_iph_count, 2); CHECK_EQ_I(errno, EINVAL);
    errno = 0; _swab(p, p, 2); CHECK_EQ_I(tc_iph_count, 3); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(zero_valid_control); TC_RUN(zero_null_src); TC_RUN(zero_null_dest); TC_RUN(zero_both_null); TC_RUN(two_null_combos);
    return tc_end();
}
