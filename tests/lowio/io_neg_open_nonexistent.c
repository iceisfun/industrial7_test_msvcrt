/* TC
 * id:    io_neg_open_nonexistent
 * area:  lowio
 * funcs: _open
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open; errno ENOENT = file or path not found
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

#define N "tc_io_neg_open_nonexistent_" TC_CRT_DLL ".tmp"
TC_CASE(rdonly) { int fd; rm(N); errno = 0; fd = _open(N, _O_RDONLY); CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); if (fd >= 0) _close(fd); }
TC_CASE(rdwr)   { int fd; rm(N); errno = 0; fd = _open(N, _O_RDWR);   CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); if (fd >= 0) _close(fd); }
TC_CASE(missing_dir)
{
    int fd; errno = 0;
    fd = _open("tc_io_no_such_dir_1\\f.tmp", _O_RDONLY);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT);
    errno = 0;
    fd = _open("tc_io_no_such_dir_1\\f.tmp", _O_CREAT | _O_WRONLY, _S_IWRITE);   /* even with O_CREAT the directory is not created */
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT);
    if (fd >= 0) _close(fd);
}
int main(void) { tc_begin(); TC_RUN(rdonly); TC_RUN(rdwr); TC_RUN(missing_dir); return tc_end(); }
