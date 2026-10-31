/* TC
 * id:    io_neg_lseek_pipe_probe
 * area:  lowio
 * funcs: _pipe _lseek _lseeki64 _tell _filelength _eof _read _write _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lseek: 'On devices incapable of seeking (such as terminals and printers), the return value is undefined'
 * note:  PROBE: _lseek/_lseeki64/_tell/_filelength/_eof on the read end of an anonymous pipe. Records rc/errno as TC NOTE lines (candidates: -1/EINVAL, -1/ESPIPE, or success). Asserts only no crash and that the pipe data stays readable afterwards.
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

#define N "tc_io_neg_lseek_pipe_probe_" TC_CRT_DLL ".tmp"

TC_CASE(pipe_seek)
{
    int fds[2], r, e; char buf[8];
    CHECK(tc_iph_install());
    r = _pipe(fds, 4096, _O_BINARY);
    CHECK_EQ_I(r, 0);
    if (r != 0) return;
    CHECK_EQ_I(_write(fds[1], "abc", 3), 3);
    errno = 0; r = _lseek(fds[0], 0, SEEK_CUR); e = errno; note("lseek_cur", r); note("lseek_errno", e);
    errno = 0; r = (int)_lseeki64(fds[0], 0, SEEK_CUR); e = errno; note("lseeki64_cur", r); note("lseeki64_errno", e);
    errno = 0; r = _tell(fds[0]); e = errno; note("tell", r); note("tell_errno", e);
    errno = 0; r = _filelength(fds[0]); e = errno; note("filelength", r); note("filelength_errno", e);
    errno = 0; r = _eof(fds[0]); e = errno; note("eof", r); note("eof_errno", e);
    memset(buf, 0, sizeof buf);
    r = _read(fds[0], buf, 3);
    CHECK_EQ_I(r, 3);
    CHECK_MEM(buf, "abc", 3);
    _close(fds[0]); _close(fds[1]);
}
int main(void) { tc_begin(); TC_RUN(pipe_seek); return tc_end(); }
