/* TC
 * id:    io_neg_pipe_read_from_write_end_probe
 * area:  lowio
 * funcs: _pipe _read _write _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _pipe, _read (not specified for the wrong pipe end)
 * note:  PROBE: _read on the write end of an anonymous pipe (with 1 byte pending). Expectation: -1 (Win32 ReadFile fails with ACCESS_DENIED, mapped to EBADF); errno recorded only.
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

#define N "tc_io_neg_pipe_read_from_write_end_probe_" TC_CRT_DLL ".tmp"

TC_CASE(read_from_write_end)
{
    int fds[2], r; char buf[4];
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY); CHECK_EQ_I(r, 0); if (r) return;
    CHECK_EQ_I(_write(fds[1], "z", 1), 1);
    errno = 0; r = _read(fds[1], buf, 1);
    note("errno", errno); CHECK_EQ_I(r, -1);
    _close(fds[0]); _close(fds[1]);
}
int main(void) { tc_begin(); TC_RUN(read_from_write_end); return tc_end(); }
