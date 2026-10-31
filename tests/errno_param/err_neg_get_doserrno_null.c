/* TC
 * id:    err_neg_get_doserrno_null
 * area:  errno_param
 * funcs: 
 * procs: _get_doserrno _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_doserrno: pValue NULL -> invalid parameter handler, EINVAL
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _get_doserrno, (unsigned long *));

TC_CASE(null_ptr)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = tc_p__get_doserrno(NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_doserrno);
    if (!tc_p__get_doserrno || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(null_ptr);
    errno = 0;
    return tc_end();
}
