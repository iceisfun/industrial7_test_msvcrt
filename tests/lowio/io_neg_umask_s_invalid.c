/* TC
 * id:    io_neg_umask_s_invalid
 * area:  lowio
 * funcs: _umask_s _umask _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _umask_s (EINVAL for NULL pOldMode); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(null_oldmode)
{
    int old = _umask(0), r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _umask_s(_S_IWRITE, NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _umask(old);
}
int main(void) { tc_begin(); TC_RUN(null_oldmode); return tc_end(); }
