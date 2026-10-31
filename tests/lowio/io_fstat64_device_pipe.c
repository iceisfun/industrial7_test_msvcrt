/* TC
 * id:    io_fstat64_device_pipe
 * area:  lowio
 * funcs: _fstat64 _open _pipe _close
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fstat (_S_IFCHR character special/device, _S_IFIFO FIFO/pipe)
 * note:  st_size of devices/pipes is deliberately not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

TC_CASE(nul_is_char)
{
    struct _stat64 st; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(fd, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFCHR);
    _close(fd);
}
TC_CASE(pipe_is_fifo)
{
    struct _stat64 st; int fds[2] = { -1, -1 };
    CHECK_EQ_I(_pipe(fds, 4096, _O_BINARY), 0);
    if (fds[0] < 0) return;
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(fds[0], &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFIFO);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_fstat64(fds[1], &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFIFO);
    _close(fds[0]); _close(fds[1]);
}
int main(void) { tc_begin(); TC_RUN(nul_is_char); TC_RUN(pipe_is_fifo); return tc_end(); }
