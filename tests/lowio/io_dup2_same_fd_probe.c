/* TC
 * id:    io_dup2_same_fd_probe
 * area:  lowio
 * funcs: _dup2 _open _close _write _lseek
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _dup2 (returns 0 on success); same-descriptor behaviour is not documented
 * note:  Probes _dup2(fd, fd) on an open fd: expected to return 0 and leave fd open and usable (not closed by the implicit close-then-duplicate).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#define N "tc_io_dup2_same_fd_probe_1.tmp"
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
TC_CASE(same_fd)
{
    int fd; volatile int v;
    rm(N); fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3); if (fd < 0) return;
    v = fd;
    CHECK_EQ_I(_dup2(fd, v), 0);
    CHECK_EQ_I(_write(fd, "ab", 2), 2);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 2);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(same_fd); rm(N); return tc_end(); }
