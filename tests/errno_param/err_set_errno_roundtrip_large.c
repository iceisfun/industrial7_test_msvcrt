/* TC
 * id:    err_set_errno_roundtrip_large
 * area:  errno_param
 * funcs: 
 * procs: _set_errno _get_errno
 * crt:   >=100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _set_errno
 * note:  PROBE: _set_errno does not validate or clamp the value (stores any int).
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(errno_t, _set_errno, (int));
TC_DECL_PROC(errno_t, _get_errno, (int *));

static void rt(int v)
{
    int got = 12345; errno_t r;
    r = tc_p__set_errno(v); CHECK_EQ_I(r, 0);
    r = tc_p__get_errno(&got); CHECK_EQ_I(r, 0); CHECK_EQ_I(got, v); CHECK_EQ_I(errno, v);
}
TC_CASE(v9999)   { rt(9999); }
TC_CASE(vneg1)   { rt(-1); }
TC_CASE(vintmax) { rt(INT_MAX); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_errno); TC_LOAD_PROC(_get_errno);
    if (!tc_p__set_errno || !tc_p__get_errno) { tc_write("TC FAIL " TC_TEST_ID "::__setup missing exports\n"); return 1; }
    TC_RUN(v9999); TC_RUN(vneg1); TC_RUN(vintmax);
    errno = 0;
    return tc_end();
}
