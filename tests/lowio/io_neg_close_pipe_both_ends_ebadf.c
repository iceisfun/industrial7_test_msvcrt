/* TC
 * id:    io_neg_close_pipe_both_ends_ebadf
 * area:  lowio
 * funcs: _pipe _close _read _write
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _close / _read / _write: EBADF for an invalid fd; _read returns 0 at end of file
 * note:  Closed pipe ends: reading from a pipe whose write end is closed yields 0 (EOF), ops on closed fds fail with EBADF, double close fails with EBADF + handler. (Reading from the write end is in io_neg_pipe_read_from_write_end_probe.)
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

#define N "tc_io_neg_close_pipe_both_ends_ebadf_" TC_CRT_DLL ".tmp"

TC_CASE(eof_after_writer_closed)
{
    int fds[2], r; char buf[4];
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY); CHECK_EQ_I(r, 0); if (r) return;
    CHECK_EQ_I(_close(fds[1]), 0);
    errno = 0;
    r = _read(fds[0], buf, 4);
    CHECK_EQ_I(r, 0);
    _close(fds[0]);
}
TC_CASE(write_closed_end)
{
    int fds[2], r;
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY); CHECK_EQ_I(r, 0); if (r) return;
    _close(fds[1]);
    tc_iph_count = 0; errno = 0;
    r = _write(fds[1], "a", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    _close(fds[0]);
}
TC_CASE(double_close_read_end)
{
    int fds[2], r;
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY); CHECK_EQ_I(r, 0); if (r) return;
    CHECK_EQ_I(_close(fds[0]), 0);
    tc_iph_count = 0; errno = 0;
    r = _close(fds[0]);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
    _close(fds[1]);
}
TC_CASE(read_closed_read_end)
{
    int fds[2], r; char buf[4];
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY); CHECK_EQ_I(r, 0); if (r) return;
    _close(fds[0]);
    errno = 0;
    r = _read(fds[0], buf, 4);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    _close(fds[1]);
}
int main(void)
{
    tc_begin();
    TC_RUN(eof_after_writer_closed); TC_RUN(write_closed_end); TC_RUN(double_close_read_end);
    TC_RUN(read_closed_read_end);
    return tc_end();
}
