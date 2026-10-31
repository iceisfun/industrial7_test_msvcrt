/* TC
 * id:    io_neg_open_excl_exists
 * area:  lowio
 * funcs: _open _close
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open: _O_EXCL returns an error value if the file specified by filename exists; errno EEXIST
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

#define N "tc_io_neg_open_excl_exists_" TC_CRT_DLL ".tmp"
TC_CASE(excl_exists)
{
    int fd1, fd2, fd3;
    mk(N, "x", 1);
    fd1 = _open(N, _O_RDONLY | _O_BINARY);      /* remember next free fd number via a probe open/close */
    CHECK(fd1 >= 3);
    if (fd1 >= 0) _close(fd1);
    errno = 0;
    fd2 = _open(N, _O_CREAT | _O_EXCL | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd2, -1);
    CHECK_EQ_I(errno, EEXIST);
    if (fd2 >= 0) _close(fd2);
    fd3 = _open(N, _O_RDONLY | _O_BINARY);
    CHECK_EQ_I(fd3, fd1);
    if (fd3 >= 0) _close(fd3);
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(excl_exists); return tc_end(); }
