/* TC
 * id:    io_open_rdonly_existing
 * area:  lowio
 * funcs: _open _close _read _write
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _open, _read; _write on O_RDONLY fd fails with EBADF
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

#define N "tc_io_open_rdonly_existing_" TC_CRT_DLL ".tmp"
TC_CASE(read_all)
{
    char b[16] = {0}; int fd;
    mk(N, "abcdef", 6);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    CHECK_EQ_I(_read(fd, b, 16), 6);
    CHECK_MEM(b, "abcdef", 6);
    CHECK_EQ_I(_read(fd, b, 16), 0);
    CHECK_EQ_I(_close(fd), 0);
    rm(N);
}
TC_CASE(write_on_rdonly_fd)
{
    int fd; mk(N, "abcdef", 6);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    if (fd >= 0) {
        char c[8];
        errno = 0;
        CHECK_EQ_I(_write(fd, "x", 1), -1);   /* nothing may be written through a read-only handle */
        _close(fd);
        CHECK_EQ_I(slurp(N, c, 8), 6);
        CHECK_MEM(c, "abcdef", 6);
    } else CHECK(0);
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(read_all); TC_RUN(write_on_rdonly_fd); return tc_end(); }
