/* TC
 * id:    gio_eof_wronly_probe
 * area:  lowio
 * funcs: _eof _open _write _lseek _close
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none
 * note:  PROBES that _eof works on a write-only descriptor (it only uses seek/length, no read access): 1 at end, 0 after rewind.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
#define N "tc_gio_eof_wronly_probe.tmp"
TC_CASE(wronly_fd_eof)
{
    int fd; rm(N);
    fd = _open(N, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcd", 4), 4);
    CHECK_EQ_I(_eof(fd), 1);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_eof(fd), 0);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(wronly_fd_eof); rm(N); return tc_end(); }
