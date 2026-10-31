/* TC
 * id:    err_neg_wcserror_s_bad_args
 * area:  errno_param
 * kind:  negative
 * funcs: _wcserror_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _wcserror_s (NULL buffer / size 0 -> EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(null_buffer)
{
    errno_t r; tc_iph_count = 0; errno = 0;
    r = _wcserror_s(NULL, 10, ENOENT);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size)
{
    wchar_t buf[4] = { L'x', L'x', L'x', L'x' }; errno_t r;
    tc_iph_count = 0;
    r = _wcserror_s(buf, 0, ENOENT);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(buf[0], L'x');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(null_buffer); TC_RUN(zero_size);
    return tc_end();
}
