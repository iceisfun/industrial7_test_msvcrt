/* TC
 * id:    pfw_neg_setmode_bad_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: _setmode _open _close _unlink
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _setmode: invalid file descriptor -> invalid parameter handler, errno EBADF, returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FN "tc_pfw_neg_setmode_bad_fd_a.tmp"
static volatile int fdm1 = -1, fdbig = 9999;

TC_CASE(fd_minus_1)
{ int r; tc_iph_count = 0; errno = 0; r = _setmode(fdm1, _O_BINARY); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fd_9999)
{ int r; tc_iph_count = 0; errno = 0; r = _setmode(fdbig, _O_BINARY); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(closed_fd)
{
    int fd = _open(FN, _O_CREAT | _O_RDWR | _O_TRUNC, _S_IREAD | _S_IWRITE); int r;
    if (fd < 0) TC_SKIP("cannot create temp file");
    _close(fd); _unlink(FN);
    tc_iph_count = 0; errno = 0; r = _setmode(fd, _O_BINARY);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(fd_minus_1); TC_RUN(fd_9999); TC_RUN(closed_fd);
    return tc_end();
}
