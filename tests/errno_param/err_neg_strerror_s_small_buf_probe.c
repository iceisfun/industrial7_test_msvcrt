/* TC
 * id:    err_neg_strerror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strerror_s
 * note:  PROBE: whether a too-small buffer yields an error code (ERANGE assumed) or truncation; and size-1 behaviour. Handler installed so a fatal default does not kill the test.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(size5_enoent)
{
    char buf[16]; errno_t r;
    memset(buf, 'x', sizeof buf);
    tc_iph_count = 0;
    r = strerror_s(buf, 5, ENOENT);
    CHECK(memchr(buf, 0, 5) != NULL);
    CHECK(r == 0 || r == ERANGE || r == EINVAL);
}
TC_CASE(size5_returns_error)
{
    char buf[16]; errno_t r;
    memset(buf, 'x', sizeof buf);
    r = strerror_s(buf, 5, ENOENT);
    CHECK(r != 0);   /* guess: ERANGE rather than silent truncation */
}
TC_CASE(size1)
{
    char buf[4]; errno_t r;
    memset(buf, 'x', sizeof buf);
    tc_iph_count = 0;
    r = strerror_s(buf, 1, ENOENT);
    CHECK(buf[0] == 0 || r == EINVAL || r == ERANGE);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(size5_enoent); TC_RUN(size5_returns_error); TC_RUN(size1);
    return tc_end();
}
