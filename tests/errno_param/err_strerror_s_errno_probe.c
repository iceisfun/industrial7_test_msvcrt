/* TC
 * id:    err_strerror_s_errno_probe
 * area:  errno_param
 * funcs: strerror_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror_s (silent about errno on success)
 * note:  PROBE: a successful strerror_s leaves errno unchanged (not documented either way).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(errno_unchanged)
{
    char buf[128]; errno_t r;
    errno = ERANGE;
    r = strerror_s(buf, sizeof buf, ENOENT);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, ERANGE);
}
int main(void)
{
    tc_begin();
    TC_RUN(errno_unchanged);
    return tc_end();
}
