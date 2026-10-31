/* TC
 * id:    prc_neg_fmode_set_invalid
 * area:  process
 * procs: __p__fmode _set_fmode _get_fmode
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode: invalid mode => EINVAL and invalid parameter handler
 */
#include "tc.h"
#include <fcntl.h>
#include <errno.h>

TC_DECL_PROC(int *, __p__fmode, (void));
TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int, _get_fmode, (int *));

TC_CASE(bad_value)
{
    int before = -1, after = -1;
    CHECK_EQ_I(tc_p__get_fmode(&before), 0);
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__set_fmode(0x1234), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__get_fmode(&after), 0);
    CHECK_EQ_I(after, before);
}
TC_CASE(minus_one)
{
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__set_fmode(-1), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_get_fmode);
    if (!tc_p__set_fmode || !tc_p__get_fmode || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(bad_value); TC_RUN(minus_one);
    return tc_end();
}
