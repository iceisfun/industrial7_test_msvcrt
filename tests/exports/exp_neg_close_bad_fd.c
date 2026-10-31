/* TC
 * id:    exp_neg_close_bad_fd
 * area:  exports
 * kind:  negative
 * funcs: _close _open _unlink
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _close: invalid descriptor -> invalid parameter handler (VS2005+), -1, EBADF
 * note:  msvcrt.dll: -1/EBADF only; iph case guarded by TC_AT_LEAST(80).
 */
#include "tc.h"
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

TC_CASE(minus1_ebadf)
{ volatile int fd = -1; errno = 0; CHECK_EQ_I(_close(fd), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(closed_fd_double_close_ebadf)
{
    int fd = _open("tc_exp_neg_close.tmp", _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    CHECK_EQ_I(_close(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    _unlink("tc_exp_neg_close.tmp");
}
TC_CASE(huge_fd_ebadf)
{ volatile int fd = 100000; errno = 0; CHECK_EQ_I(_close(fd), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(iph_called_on_80plus)
{
    volatile int fd = -1;
    if (!TC_AT_LEAST(80)) TC_SKIP("no iph before VS2005");
    tc_iph_count = 0;
    _close(fd);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install();
    TC_RUN(minus1_ebadf); TC_RUN(closed_fd_double_close_ebadf); TC_RUN(huge_fd_ebadf); TC_RUN(iph_called_on_80plus);
    return tc_end();
}
