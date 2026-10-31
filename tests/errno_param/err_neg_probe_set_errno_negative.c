/* TC
 * id:    err_neg_probe_set_errno_negative
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: _set_errno _get_errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_errno (no range validation documented)
 * note:  PROBE: _set_errno(-1) and _set_errno(INT_MIN): the docs list no failure case. Most likely 0 is returned, the value is stored
 *        verbatim and the invalid-parameter handler does not run. Asserted as such; deviations are the finding.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(errno_t, _set_errno, (int));
TC_DECL_PROC(errno_t, _get_errno, (int *));

TC_CASE(minus_one)
{
    errno_t r; int v = 12345; tc_iph_count = 0;
    r = tc_p__set_errno(-1);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, -1);
    CHECK_EQ_I(tc_p__get_errno(&v), 0); CHECK_EQ_I(v, -1);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(int_min)
{
    errno_t r; int v = 12345; tc_iph_count = 0;
    r = tc_p__set_errno(INT_MIN);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, INT_MIN);
    CHECK_EQ_I(tc_p__get_errno(&v), 0); CHECK_EQ_I(v, INT_MIN);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_errno); TC_LOAD_PROC(_get_errno);
    if (!tc_p__set_errno || !tc_p__get_errno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(minus_one); TC_RUN(int_min);
    errno = 0;
    return tc_end();
}
