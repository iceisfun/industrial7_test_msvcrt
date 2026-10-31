/* TC
 * id:    io_commit_rdonly_probe
 * area:  lowio
 * funcs: _commit _open _close
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _commit
 * note:  conf probe: _commit on a descriptor opened _O_RDONLY. FlushFileBuffers needs GENERIC_WRITE so it fails with ERROR_ACCESS_DENIED; the CRT maps that to -1 (errno EBADF). Asserting -1 only.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, int ro)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    if (ro) SetFileAttributesA(n, FILE_ATTRIBUTE_READONLY);
}

static const char *volatile fn = "tc_io_commit_rdonly_probe.tmp";
TC_CASE(commit_rdonly_fd)
{
    int fd;
    mk(fn, 0);
    fd = _open(fn, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) { rm(fn); return; }
    CHECK_EQ_I(_commit(fd), -1);
    _close(fd);
    rm(fn);
}
int main(void) { tc_begin(); TC_RUN(commit_rdonly_fd); return tc_end(); }
