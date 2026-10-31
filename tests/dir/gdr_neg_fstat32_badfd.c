/* TC
 * id:    gdr_neg_fstat32_badfd
 * area:  dir
 * kind:  negative
 * funcs: _fstat32 _fstat32i64 _open _close
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fstat: EBADF invalid file descriptor (invalid parameter handler invoked)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static volatile int v_fd;
#define BEGIN tc_iph_install(); tc_iph_count = 0; errno = 0
#define EXPECT(r) CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count >= 1)

TC_CASE(minus_one)
{
    struct _stat32 a; struct _stat32i64 b; v_fd = -1;
    BEGIN; EXPECT(_fstat32(v_fd, &a));
    BEGIN; EXPECT(_fstat32i64(v_fd, &b));
}
TC_CASE(out_of_range)
{
    struct _stat32 a; struct _stat32i64 b; v_fd = 0x7fffffff;
    BEGIN; EXPECT(_fstat32(v_fd, &a));
    BEGIN; EXPECT(_fstat32i64(v_fd, &b));
}
TC_CASE(closed_fd)
{
    struct _stat32 a; struct _stat32i64 b; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    v_fd = fd;
    BEGIN; EXPECT(_fstat32(v_fd, &a));
    BEGIN; EXPECT(_fstat32i64(v_fd, &b));
}
TC_CASE(double_close)
{
    struct _stat32 a; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    v_fd = fd;
    BEGIN; CHECK_EQ_I(_close(v_fd), -1); CHECK_EQ_I(errno, EBADF);
    BEGIN; EXPECT(_fstat32(v_fd, &a));
}
int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(out_of_range); TC_RUN(closed_fd); TC_RUN(double_close); return tc_end(); }
