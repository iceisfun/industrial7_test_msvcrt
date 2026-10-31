/* TC
 * id:    io_open_pmode_creat_readonly
 * area:  lowio
 * funcs: _open _close _write _access _chmod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _open pmode _S_IREAD; _access
 * note:  The _write succeeding on the fd returned by the creating _open is Win32 CreateFile behaviour (read-only attribute applies to later opens), not spelled out in the _open docs.
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

#define N "tc_io_open_pmode_creat_readonly_" TC_CRT_DLL ".tmp"
TC_CASE(ro_create)
{
    int fd;
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);     /* the handle itself is opened for writing */
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    CHECK_EQ_I(_access(N, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK_EQ_I(_access(N, 4), 0);
    CHECK_EQ_I(_chmod(N, _S_IREAD | _S_IWRITE), 0);
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(ro_create); return tc_end(); }
