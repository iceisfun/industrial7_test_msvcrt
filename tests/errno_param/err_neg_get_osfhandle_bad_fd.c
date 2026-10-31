/* TC
 * id:    err_neg_get_osfhandle_bad_fd
 * area:  errno_param
 * kind:  negative
 * funcs: _get_osfhandle _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_osfhandle: returns INVALID_HANDLE_VALUE (-1) on invalid fd
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile int v_fd;

TC_CASE(minus_one) { intptr_t h; v_fd = -1; errno = 0; h = _get_osfhandle(v_fd); CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(huge_fd) { intptr_t h; v_fd = 9999; errno = 0; h = _get_osfhandle(v_fd); CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, EBADF); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(minus_one); TC_RUN(huge_fd);
    return tc_end();
}
