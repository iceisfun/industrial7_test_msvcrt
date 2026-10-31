/* TC
 * id:    io_neg_open_readonly_for_write
 * area:  lowio
 * funcs: _open _close _chmod _unlink
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open: EACCES; _chmod
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, (DWORD)cap, &r, NULL); CloseHandle(h); return (int)r;
}
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define N "tc_io_neg_open_readonly_for_write_" TC_CRT_DLL ".tmp"
TC_CASE(ro)
{
    int fd;
    mk(N, "data", 4);
    CHECK_EQ_I(_chmod(N, _S_IREAD), 0);
    errno = 0;
    fd = _open(N, _O_WRONLY | _O_BINARY);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EACCES);
    if (fd >= 0) _close(fd);
    errno = 0;
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EACCES);
    if (fd >= 0) _close(fd);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0);
    CHECK_EQ_I(_chmod(N, _S_IREAD | _S_IWRITE), 0);
    CHECK_EQ_I(_unlink(N), 0);
}
int main(void) { tc_begin(); TC_RUN(ro); return tc_end(); }
