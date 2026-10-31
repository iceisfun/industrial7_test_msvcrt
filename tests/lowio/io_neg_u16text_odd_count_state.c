/* TC
 * id:    io_neg_u16text_odd_count_state
 * area:  lowio
 * funcs: _open _read _write _setmode _close _filelength
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _read / _write: in Unicode (UTF-16) mode count must be even, else EINVAL
 * note:  Extends io_neg_u16text_odd_bytes: odd and tiny counts, file length unchanged after a failed write, and the same fd still works with an even count afterwards. The UTF-8 text mode cases assert only no crash and rc >= -1 (behaviour of tiny counts not documented).
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

#define N "tc_io_neg_u16text_odd_count_state_" TC_CRT_DLL ".tmp"

static int mkfd(int mode)
{
    int fd;
    rm(N); mk(N, "A\0B\0C\0D\0", 8);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { int pm = _setmode(fd, mode); CHECK_EQ_I(pm, _O_BINARY); }
    return fd;
}
TC_CASE(u16_read_count1)
{
    int fd, r; char buf[8];
    CHECK(tc_iph_install());
    fd = mkfd(_O_U16TEXT); if (fd < 0) { rm(N); return; }
    memset(buf, 0x5a, sizeof buf);
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I((unsigned char)buf[0], 0x5a);
    _close(fd); rm(N);
}
TC_CASE(u16_read_count3_then_even)
{
    int fd, r; char buf[8];
    CHECK(tc_iph_install());
    fd = mkfd(_O_U16TEXT); if (fd < 0) { rm(N); return; }
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, 3);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    memset(buf, 0, sizeof buf);
    r = _read(fd, buf, 2);
    CHECK_EQ_I(r, 2);
    CHECK_EQ_I(buf[0], 'A'); CHECK_EQ_I(buf[1], 0);
    _close(fd); rm(N);
}
TC_CASE(wtext_write_odd_then_even)
{
    int fd, r; char buf[4] = { 'x', 0, 'y', 0 }; long len;
    CHECK(tc_iph_install());
    fd = mkfd(_O_WTEXT); if (fd < 0) { rm(N); return; }
    len = _filelength(fd);
    tc_iph_count = 0; errno = 0;
    r = _write(fd, buf, 3);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_filelength(fd), len);
    r = _write(fd, buf, 4);
    CHECK(r > 0);
    _close(fd); rm(N);
}
TC_CASE(u8text_tiny_read)
{
    int fd, r; char buf[8];
    CHECK(tc_iph_install());
    fd = mkfd(_O_U8TEXT); if (fd < 0) { rm(N); return; }
    r = _read(fd, buf, 0); CHECK(r >= -1);
    r = _read(fd, buf, 1); CHECK(r >= -1);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(u16_read_count1); TC_RUN(u16_read_count3_then_even); TC_RUN(wtext_write_odd_then_even);
    TC_RUN(u8text_tiny_read);
    return tc_end();
}
