/* TC
 * id:    err_neg_close_bad_fd
 * area:  errno_param
 * kind:  negative
 * funcs: _close _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _close, Parameter validation
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

static volatile int v_fd;

TC_CASE(minus_one) { int r; v_fd = -1; errno = 0; r = _close(v_fd); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(huge_fd) { int r; v_fd = 9999; errno = 0; r = _close(v_fd); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(double_close)
{
    int fd, r; v_fd = 0;
    fd = _open("tc_err_neg_close_bad_fd_a" TC_S1(TC_CRT_VER) ".tmp", _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    v_fd = fd;
    CHECK_EQ_I(_close(v_fd), 0);
    errno = 0; r = _close(v_fd);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    _unlink("tc_err_neg_close_bad_fd_a" TC_S1(TC_CRT_VER) ".tmp");
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(minus_one); TC_RUN(huge_fd); TC_RUN(double_close);
    return tc_end();
}
