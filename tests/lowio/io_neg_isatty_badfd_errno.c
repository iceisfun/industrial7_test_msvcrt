/* TC
 * id:    io_neg_isatty_badfd_errno
 * area:  lowio
 * funcs: _isatty _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _isatty
 * note:  Probes: _isatty(-1) sets errno = EBADF and calls the invalid parameter handler (likely for VS2005+ CRTs; not documented).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#define N "tc_io_neg_isatty_badfd_errno_" TC_CRT_DLL ".tmp"

TC_CASE(errno_ebadf)
{
    CHECK(tc_iph_install());
    errno = 0;
    CHECK_EQ_I(_isatty(-1), 0);
    CHECK_EQ_I(errno, EBADF);
}
TC_CASE(handler_called)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(_isatty(-1), 0);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(errno_ebadf);
    TC_RUN(handler_called);
    return tc_end();
}
