/* TC
 * id:    io_neg_eof_badfd
 * area:  lowio
 * funcs: _eof _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _eof: returns -1 and sets errno to EBADF for an invalid descriptor
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <winioctl.h>
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)

static const char *volatile fname = "tc_io_neg_eof_badfd_" TC_STR(TC_CRT_VER) ".tmp";
TC_CASE(minus1) { int c = tc_iph_count; errno = 0; CHECK_EQ_I(_eof(-1), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c); }
TC_CASE(closed)
{
    int c, fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    c = tc_iph_count; errno = 0;
    CHECK_EQ_I(_eof(fd), -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count > c);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(minus1); TC_RUN(closed);
    _unlink(fname);
    return tc_end();
}
