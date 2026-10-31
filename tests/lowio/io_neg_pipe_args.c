/* TC
 * id:    io_neg_pipe_args
 * area:  lowio
 * funcs: _pipe _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _pipe: invalid parameter -> EINVAL and invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_neg_pipe_args_" TC_CRT_DLL ".tmp"

TC_CASE(null_pfds)
{
    int r; CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _pipe(NULL, 4096, _O_BINARY);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_pfds);
    return tc_end();
}
