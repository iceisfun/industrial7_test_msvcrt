/* TC
 * id:    err_neg_underscore_strerror_s_bad_args
 * area:  errno_param
 * kind:  negative
 * funcs: _strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strerror_s (NULL buffer / size 0 -> EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(null_buffer)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _strerror_s(NULL, 10, "x");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size)
{
    char buf[8]; errno_t r;
    memset(buf, 'x', sizeof buf);
    tc_iph_count = 0;
    r = _strerror_s(buf, 0, "x");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(buf[0], 'x');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_buffer); TC_RUN(zero_size);
    return tc_end();
}
