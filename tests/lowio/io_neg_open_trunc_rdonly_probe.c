/* TC
 * id:    io_neg_open_trunc_rdonly_probe
 * area:  lowio
 * funcs: _open _close _filelength
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: _O_TRUNC ... 'The result is undefined if you specify _O_TRUNC with _O_RDONLY'
 * note:  PROBE: the docs call _O_RDONLY|_O_TRUNC undefined. Records whether _open succeeds and whether the file contents were truncated. Asserts only no crash and that a failure is exactly -1.
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

#define N "tc_io_neg_open_trunc_rdonly_probe_" TC_CRT_DLL ".tmp"

TC_CASE(trunc_rdonly)
{
    int fd, e; char buf[8];
    CHECK(tc_iph_install());
    mk(N, "hello", 5);
    errno = 0;
    fd = _open(N, _O_RDONLY | _O_TRUNC | _O_BINARY);
    e = errno;
    note("fd", fd);
    if (fd < 0) { CHECK_EQ_I(fd, -1); note("errno", e); }
    else { CHECK(fd >= 3); note("len_while_open", _filelength(fd)); _close(fd); }
    note("len_after", slurp(N, buf, sizeof buf));
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(trunc_rdonly); return tc_end(); }
