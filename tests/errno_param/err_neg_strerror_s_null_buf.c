/* TC
 * id:    err_neg_strerror_s_null_buf
 * area:  errno_param
 * kind:  negative
 * funcs: strerror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strerror_s (NULL buffer -> EINVAL), Parameter validation
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(null_buffer)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = strerror_s(NULL, 10, ENOENT);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_buffer);
    return tc_end();
}
