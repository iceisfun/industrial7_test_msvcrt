/* TC
 * id:    io_neg_read_state_after_failure
 * area:  lowio
 * funcs: _open _read _write _lseek _tell _eof _filelength _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read / _write: EBADF for wrong access mode; NULL buffer -> EINVAL and handler; returns -1
 * note:  A failed _read/_write must leave the file pointer, file length and caller buffer untouched. Zero-count write returns 0; read at EOF returns 0 with _eof()==1.
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

#define N "tc_io_neg_read_state_after_failure_" TC_CRT_DLL ".tmp"

static int op(void)
{
    int fd;
    rm(N); mk(N, "0123456789", 10);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    return fd;
}
TC_CASE(read_wronly)
{
    int fd, r; char buf[8];
    CHECK(tc_iph_install());
    rm(N); mk(N, "abcd", 4);
    fd = _open(N, _O_WRONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    memset(buf, 0x5a, sizeof buf); errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I((unsigned char)buf[0], 0x5a); CHECK_EQ_I((unsigned char)buf[3], 0x5a);
    CHECK_EQ_I(_tell(fd), 0);
    _close(fd); rm(N);
}
TC_CASE(write_null)
{
    int fd = op(), r;
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    tc_iph_count = 0; errno = 0;
    r = _write(fd, NULL, 5);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_filelength(fd), 10); CHECK_EQ_I(_tell(fd), 0);
    _close(fd); rm(N);
}
TC_CASE(write_zero)
{
    int fd = op(), r; char buf[4] = "abc";
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    tc_iph_count = 0; errno = 0;
    r = _write(fd, buf, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(_filelength(fd), 10);
    _close(fd); rm(N);
}
TC_CASE(read_at_eof)
{
    int fd = op(), r; char buf[8];
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    _lseek(fd, 0, SEEK_END);
    errno = 0; tc_iph_count = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_wronly); TC_RUN(write_null);
    TC_RUN(write_zero); TC_RUN(read_at_eof);
    return tc_end();
}
