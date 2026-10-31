/* TC
 * id:    err_neg_iph_exactly_once_io
 * area:  errno_param
 * kind:  negative
 * funcs: fclose _close _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fclose (NULL stream), _close (invalid file descriptor): parameter validation invokes the handler
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static volatile int v_bad_fd = -1;

TC_CASE(fclose_null)  { int r; tc_iph_count = 0; errno = 0; r = fclose((FILE * volatile)NULL); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(close_bad_fd) { int r; tc_iph_count = 0; errno = 0; r = _close(v_bad_fd); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no handler api\n"); return 1; }
    TC_RUN(fclose_null); TC_RUN(close_bad_fd);
    return tc_end();
}
