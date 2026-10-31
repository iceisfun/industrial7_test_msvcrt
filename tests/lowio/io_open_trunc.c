/* TC
 * id:    io_open_trunc
 * area:  lowio
 * funcs: _open _close _filelength
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _open _O_TRUNC; _filelength
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

#define N "tc_io_open_trunc_" TC_CRT_DLL ".tmp"
TC_CASE(trunc)
{
    int fd;
    mk(N, "0123456789", 10);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_filelength(fd), 10); CHECK_EQ_I(_close(fd), 0); }
    fd = _open(N, _O_WRONLY | _O_TRUNC | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_filelength(fd), 0); CHECK_EQ_I(_close(fd), 0); }
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(trunc); return tc_end(); }
