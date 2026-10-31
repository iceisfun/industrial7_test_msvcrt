/* TC
 * id:    io_neg_chsize_readonly_probe
 * area:  lowio
 * funcs: _chsize _chsize_s _open _close _filelength
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _chsize: EACCES
 * note:  probing: shrinking a file through a read-only descriptor fails with -1/EACCES (SetEndOfFile access denied); docs do not spell out this case
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#define N "tc_io_neg_chsize_readonly_probe_1.tmp"
TC_CASE(readonly_fd)
{
    int fd; mk(N, "abcdef", 6);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    errno = 0; CHECK_EQ_I(_chsize(fd, 2), -1); CHECK_EQ_I(errno, EACCES);
    CHECK_EQ_I(_chsize_s(fd, 2), EACCES);
    CHECK_EQ_I(_filelength(fd), 6);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(readonly_fd); return tc_end(); }
