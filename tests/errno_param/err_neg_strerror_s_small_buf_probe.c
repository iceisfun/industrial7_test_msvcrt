/* TC
 * id:    err_neg_strerror_s_small_buf_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs strerror_s
 * note:  Observed on Win10 22H2 19045 ucrtbase: strerror_s with a 5-byte buffer truncates the message to 4 chars + NUL and
 *        returns 0 (no ERANGE, no handler call); the old guess r != 0 was wrong.
 *        Deviates from C11 K.3.7.4.2p4 (nonzero return when the message does not fit); matches the MS docs, which document truncation. size1 and size5_enoent stay as written.
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
TC_CASE(size5_truncates_returns_0)
{
    char buf[16]; errno_t r;
    memset(buf, 'x', sizeof buf);
    tc_iph_count = 0;
    r = strerror_s(buf, 5, ENOENT);
    CHECK_EQ_I(r, 0);            /* observed: silent truncation, not ERANGE */
    CHECK_STR(buf, "No s");      /* "No such file or directory" cut to 4 chars + NUL */
    CHECK_EQ_I(tc_iph_count, 0);
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
    TC_RUN(size5_enoent); TC_RUN(size5_truncates_returns_0); TC_RUN(size1);
    return tc_end();
}
