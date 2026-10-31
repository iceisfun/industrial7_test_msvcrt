/* TC
 * id:    err_neg_close_bad_fd_iph_probe
 * area:  errno_param
 * kind:  negative
 * funcs: _close _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _close: invalid fd invokes the invalid parameter handler
 * note:  PROBE: whether the handler fires for _close(-1), _close(9999) and a double close (expected yes, exactly >=1 for each). Kept apart from the documented errno/return checks.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile int v_fd;

TC_CASE(minus_one) { tc_iph_count = 0; v_fd = -1; _close(v_fd); CHECK(tc_iph_count >= 1); }
TC_CASE(huge_fd) { tc_iph_count = 0; v_fd = 9999; _close(v_fd); CHECK(tc_iph_count >= 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(minus_one); TC_RUN(huge_fd);
    return tc_end();
}
