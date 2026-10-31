/* TC
 * id:    err_neg_strerror_s_zero_size
 * area:  errno_param
 * kind:  negative
 * funcs: strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strerror_s (sizeInBytes 0 -> EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(zero_size)
{
    char buf[8]; errno_t r;
    memset(buf, 'x', sizeof buf);
    tc_iph_count = 0; errno = 0;
    r = strerror_s(buf, 0, ENOENT);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    { size_t i; for (i = 0; i < sizeof buf; i++) CHECK_EQ_I(buf[i], 'x'); }
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(zero_size);
    return tc_end();
}
