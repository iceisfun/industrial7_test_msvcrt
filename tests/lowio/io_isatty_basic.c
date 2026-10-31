/* TC
 * id:    io_isatty_basic
 * area:  lowio
 * funcs: _isatty _open _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _isatty: nonzero if fd is associated with a character device
 * note:  NUL is a character device so _isatty returns nonzero (the CRT flags it FDEV).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
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

#define N "tc_io_isatty_basic_" TC_CRT_DLL ".tmp"

TC_CASE(regular_file)
{
    int fd; mk(N, "x", 1);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_isatty(fd), 0); _close(fd); }
    rm(N);
}
TC_CASE(nul_device)
{
    int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK(_isatty(fd) != 0); _close(fd); }
}
TC_CASE(pipe_fd)
{
    int f[2] = { -1, -1 };
    CHECK_EQ_I(_pipe(f, 256, _O_BINARY), 0);
    if (f[0] >= 0) { CHECK_EQ_I(_isatty(f[0]), 0); CHECK_EQ_I(_isatty(f[1]), 0); _close(f[0]); _close(f[1]); }
}

int main(void)
{
    tc_begin();
    TC_RUN(regular_file);
    TC_RUN(nul_device);
    TC_RUN(pipe_fd);
    return tc_end();
}
