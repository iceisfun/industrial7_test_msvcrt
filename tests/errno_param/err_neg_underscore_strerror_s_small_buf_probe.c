/* TC
 * id:    err_neg_underscore_strerror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _strerror_s
 * note:  PROBE: a too-small buffer is assumed to yield a nonzero return (ERANGE) and a NUL-terminated buffer.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(small_buffer)
{
    char buf[8]; errno_t r;
    memset(buf, 'x', sizeof buf);
    errno = ENOENT;
    r = _strerror_s(buf, 4, "abcdef");
    CHECK(memchr(buf, 0, 4) != NULL);
    CHECK(r == 0 || r == ERANGE || r == EINVAL);
}
TC_CASE(small_buffer_returns_error)
{
    char buf[8]; errno_t r;
    memset(buf, 'x', sizeof buf);
    errno = ENOENT;
    r = _strerror_s(buf, 4, "abcdef");
    CHECK(r != 0);   /* guess: ERANGE rather than silent truncation */
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(small_buffer); TC_RUN(small_buffer_returns_error);
    return tc_end();
}
