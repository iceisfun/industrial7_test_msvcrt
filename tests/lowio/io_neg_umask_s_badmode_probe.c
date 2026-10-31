/* TC
 * id:    io_neg_umask_s_badmode_probe
 * area:  lowio
 * funcs: _umask_s _umask _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _umask_s
 * note:  conf probe: _umask_s with mode bits outside _S_IREAD|_S_IWRITE (0x10000) is expected to hit the invalid-parameter handler, return EINVAL and leave the mask unchanged. Not clearly documented.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(invalid_mode_bits)
{
    int old = _umask(0), prev = -1, r;
    volatile int mode = 0x10000;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _umask_s(mode, &prev);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_umask(0), 0);   /* mask unchanged */
    _umask(old);
}
int main(void) { tc_begin(); TC_RUN(invalid_mode_bits); return tc_end(); }
