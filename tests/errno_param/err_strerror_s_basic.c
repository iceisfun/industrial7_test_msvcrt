/* TC
 * id:    err_strerror_s_basic
 * area:  errno_param
 * funcs: strerror_s strerror
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strerror_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(enoent_equals_strerror)
{
    char buf[128]; errno_t r;
    memset(buf, 'x', sizeof buf);
    r = strerror_s(buf, sizeof buf, ENOENT);
    CHECK_EQ_I(r, 0);
    CHECK_STR(buf, strerror(ENOENT));
    CHECK(buf[0] != 0);
}
TC_CASE(errnum_zero)
{
    char buf[128]; errno_t r;
    r = strerror_s(buf, sizeof buf, 0);
    CHECK_EQ_I(r, 0);
    CHECK_STR(buf, "No error");
}
TC_CASE(exact_fit)
{
    char buf[128], small[128]; errno_t r; size_t n;
    volatile int e = EINVAL;
    r = strerror_s(buf, sizeof buf, e);
    CHECK_EQ_I(r, 0);
    n = strlen(buf);
    memset(small, 'x', sizeof small);
    r = strerror_s(small, n + 1, e);
    CHECK_EQ_I(r, 0);
    CHECK_STR(small, buf);
    CHECK_EQ_I(small[n + 1], 'x');
}
int main(void)
{
    tc_begin();
    TC_RUN(enoent_equals_strerror); TC_RUN(errnum_zero); TC_RUN(exact_fit);
    return tc_end();
}
