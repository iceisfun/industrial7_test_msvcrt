/* TC
 * id:    io_neg_write_wronly_append_read
 * area:  lowio
 * funcs: _open _read _write _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read / _write: EBADF if the file handle is not valid or not opened for reading / writing
 * note:  Cross-check on append-mode and text-mode fds; failed operations must not change the file. RDWR|BINARY is the positive control.
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

#define N "tc_io_neg_write_wronly_append_read_" TC_CRT_DLL ".tmp"

TC_CASE(read_on_wronly_append)
{
    int fd, r; char buf[8];
    rm(N); mk(N, "abcd", 4);
    fd = _open(N, _O_WRONLY | _O_APPEND | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    memset(buf, 0x5a, sizeof buf); errno = 0;
    r = _read(fd, buf, 4);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I((unsigned char)buf[0], 0x5a);
    _close(fd);
    CHECK_EQ_I(slurp(N, buf, sizeof buf), 4);
    rm(N);
}
TC_CASE(write_on_rdonly_text)
{
    int fd, r; char buf[8];
    rm(N); mk(N, "abcd", 4);
    fd = _open(N, _O_RDONLY | _O_TEXT);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    errno = 0;
    r = _write(fd, "XY", 2);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF);
    _close(fd);
    memset(buf, 0, sizeof buf);
    CHECK_EQ_I(slurp(N, buf, sizeof buf), 4);
    CHECK_MEM(buf, "abcd", 4);
    rm(N);
}
TC_CASE(rdwr_binary_control)
{
    int fd, r; char buf[8];
    rm(N); mk(N, "abcd", 4);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    r = _read(fd, buf, 2); CHECK_EQ_I(r, 2); CHECK_MEM(buf, "ab", 2);
    r = _write(fd, "ZZ", 2); CHECK_EQ_I(r, 2);
    _close(fd);
    memset(buf, 0, sizeof buf);
    CHECK_EQ_I(slurp(N, buf, sizeof buf), 4);
    CHECK_MEM(buf, "abZZ", 4);
    rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_on_wronly_append); TC_RUN(write_on_rdonly_text); TC_RUN(rdwr_binary_control);
    return tc_end();
}
