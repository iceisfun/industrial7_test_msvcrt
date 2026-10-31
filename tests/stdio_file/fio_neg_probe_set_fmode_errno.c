/* TC
 * id:    fio_neg_probe_set_fmode_errno
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: _set_fmode _get_fmode
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_fmode / _get_fmode
 * note:  PROBE: errno is set to EINVAL (in addition to the EINVAL return value) when _set_fmode(0x1234) and _get_fmode(NULL) fail.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int, _get_fmode, (int *));

TC_CASE(set_errno)
{
    if (!tc_p__set_fmode) TC_SKIP("no _set_fmode");
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__set_fmode(0x1234), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(get_errno)
{
    if (!tc_p__get_fmode) TC_SKIP("no _get_fmode");
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__get_fmode(NULL), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_get_fmode);
    TC_RUN(set_errno); TC_RUN(get_errno);
    return tc_end();
}
