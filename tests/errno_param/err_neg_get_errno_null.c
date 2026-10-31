/* TC
 * id:    err_neg_get_errno_null
 * area:  errno_param
 * funcs: _set_invalid_parameter_handler
 * procs: _get_errno
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_errno: pValue NULL -> invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_errno, (int *));

TC_CASE(null_ptr)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = tc_p__get_errno(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_ptr_no_handler)
{
    int v = -1; errno_t r; tc_iph_count = 0; errno = ERANGE;
    r = tc_p__get_errno(&v);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(v, ERANGE); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_errno);
    if (!tc_p__get_errno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(null_ptr); TC_RUN(valid_ptr_no_handler);
    errno = 0;
    return tc_end();
}
