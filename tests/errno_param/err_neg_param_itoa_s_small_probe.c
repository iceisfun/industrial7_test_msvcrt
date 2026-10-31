/* TC
 * id:    err_neg_param_itoa_s_small_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _itoa_s _set_invalid_parameter_handler
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _itoa_s (error-condition table)
 * note:  PROBE: _itoa_s(12345, buf[3], 3, 10) returns ERANGE, sets errno=ERANGE and leaves buf[0]==0. The MS table may list EINVAL for a too-small buffer; real Windows decides.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(too_small_code)
{
    char b[3] = { 'x', 'x', 'x' }; errno_t r; volatile int v = 12345; tc_iph_count = 0; errno = 0;
    r = _itoa_s(v, b, sizeof b, 10);
    CHECK_EQ_I(r, ERANGE);
}
TC_CASE(too_small_errno)
{
    char b[3] = { 'x', 'x', 'x' }; volatile int v = 12345; errno = 0;
    (void)_itoa_s(v, b, sizeof b, 10);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(too_small_buffer_reset)
{
    char b[3] = { 'x', 'x', 'x' }; volatile int v = 12345;
    (void)_itoa_s(v, b, sizeof b, 10);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no handler API\n"); return 1; }
    TC_RUN(too_small_code); TC_RUN(too_small_errno); TC_RUN(too_small_buffer_reset);
    return tc_end();
}
