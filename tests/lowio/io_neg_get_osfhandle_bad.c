/* TC
 * id:    io_neg_get_osfhandle_bad
 * area:  lowio
 * funcs: _get_osfhandle _open _close _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_osfhandle ("If fd is not valid, returns INVALID_HANDLE_VALUE (-1) and sets errno to EBADF"); Parameter validation
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(minus_one)
{
    volatile int fd = -1; intptr_t h;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    h = _get_osfhandle(fd);
    CHECK_EQ_I(h, (intptr_t)INVALID_HANDLE_VALUE);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(closed_fd)
{
    const char *volatile fn = "tc_io_neg_get_osfhandle_" TC_S(TC_CRT_VER) ".tmp"; int fd; intptr_t h;
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    CHECK_EQ_I(_close(fd), 0);
    CHECK(tc_iph_install());
    errno = 0;
    h = _get_osfhandle(fd);
    CHECK_EQ_I(h, (intptr_t)INVALID_HANDLE_VALUE);
    CHECK_EQ_I(errno, EBADF);
    DeleteFileA(fn);
}
TC_CASE(huge_fd)
{
    volatile int fd = 1000000; intptr_t h;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    h = _get_osfhandle(fd);
    CHECK_EQ_I(h, (intptr_t)INVALID_HANDLE_VALUE);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(closed_fd); TC_RUN(huge_fd); return tc_end(); }
