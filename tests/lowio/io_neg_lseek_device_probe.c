/* TC
 * id:    io_neg_lseek_device_probe
 * area:  lowio
 * funcs: _open _close _lseek _filelength _tell _isatty
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lseek / _filelength: behaviour on devices incapable of seeking is undefined
 * note:  PROBE: opens the NUL device and records _lseek(fd,10,SEEK_SET), _tell, _filelength, _isatty as TC NOTE lines. Asserts only no crash and that the fd is closable.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
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
static void note(const char *k, long long v)
{
    char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}

#define N "tc_io_neg_lseek_device_probe_" TC_CRT_DLL ".tmp"

TC_CASE(nul_seek)
{
    int fd, r, e;
    CHECK(tc_iph_install());
    fd = _open("NUL", _O_RDWR | _O_BINARY);
    note("fd", fd);
    if (fd < 0) return;
    CHECK(fd >= 3);
    errno = 0; r = _lseek(fd, 10, SEEK_SET); e = errno; note("lseek", r); note("lseek_errno", e);
    errno = 0; r = _tell(fd); e = errno; note("tell", r); note("tell_errno", e);
    errno = 0; r = _filelength(fd); e = errno; note("filelength", r); note("filelength_errno", e);
    note("isatty", _isatty(fd));
    CHECK_EQ_I(_close(fd), 0);
}
int main(void) { tc_begin(); TC_RUN(nul_seek); return tc_end(); }
