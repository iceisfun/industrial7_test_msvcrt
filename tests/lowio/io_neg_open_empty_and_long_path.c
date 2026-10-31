/* TC
 * id:    io_neg_open_empty_and_long_path
 * area:  lowio
 * funcs: _open _wopen _sopen_s _creat _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open/_wopen/_creat: -1 and errno=ENOENT when path not found; _sopen_s returns errno
 * note:  Empty path and an over-MAX_PATH name must fail without invoking the invalid parameter handler (only NULL is an invalid parameter). The 5000-char and the '<' '|' names assert only rc==-1 and errno!=0 because the exact errno mapping is not documented. Also checks no fd leaks on failure.
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

#define N "tc_io_neg_open_empty_and_long_path_" TC_CRT_DLL ".tmp"
#include <share.h>

TC_CASE(empty_open)
{
    int fd;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    fd = _open("", _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
    if (fd >= 0) _close(fd);
}
TC_CASE(empty_wopen)
{
    int fd;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    fd = _wopen(L"", _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
    if (fd >= 0) _close(fd);
}
TC_CASE(empty_creat)
{
    int fd;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    fd = _creat("", _S_IWRITE);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
    if (fd >= 0) _close(fd);
}
TC_CASE(empty_sopen_s)
{
    int fd = 12345; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    r = _sopen_s(&fd, "", _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(r, ENOENT);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(tc_iph_count, 0);
    if (fd >= 0) _close(fd);
}
TC_CASE(long_path)
{
    static char p[5001]; int i, fd;
    CHECK(tc_iph_install());
    for (i = 0; i < 5000; i++) p[i] = 'a';
    p[5000] = 0;
    errno = 0;
    fd = _open(p, _O_RDONLY);
    CHECK_EQ_I(fd, -1);
    CHECK(errno != 0);
    if (fd >= 0) _close(fd);
}
TC_CASE(invalid_chars)
{
    int fd;
    CHECK(tc_iph_install());
    errno = 0;
    fd = _open("tc_io_a<b|c.tmp", _O_CREAT | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd, -1);
    CHECK(errno != 0);
    if (fd >= 0) { _close(fd); rm("tc_io_a<b|c.tmp"); }
}
TC_CASE(no_fd_leak)
{
    int a, b, fd;
    CHECK(tc_iph_install());
    mk(N, "x", 1);
    a = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(a >= 3);
    if (a < 0) { rm(N); return; }
    _close(a);
    fd = _open("", _O_RDONLY); if (fd >= 0) _close(fd);
    fd = _wopen(L"", _O_RDONLY); if (fd >= 0) _close(fd);
    fd = _creat("", _S_IWRITE); if (fd >= 0) _close(fd);
    fd = _open("tc_io_nonexistent_dir\\nofile", _O_RDONLY); if (fd >= 0) _close(fd);
    b = _open(N, _O_RDONLY | _O_BINARY);
    CHECK_EQ_I(b, a);
    if (b >= 0) _close(b);
    rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(empty_open); TC_RUN(empty_wopen); TC_RUN(empty_creat); TC_RUN(empty_sopen_s);
    TC_RUN(long_path); TC_RUN(invalid_chars); TC_RUN(no_fd_leak);
    return tc_end();
}
