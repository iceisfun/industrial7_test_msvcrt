/* TC
 * id:    io_neg_read_count_toobig_state_probe
 * area:  lowio
 * funcs: _open _read _lseek _tell _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _read (count > INT_MAX handling is not clearly documented)
 * note:  PROBE: _read with count UINT_MAX and INT_MAX+1 on a 10-byte file with a 16-byte buffer. Expectation (from UCRT validation behaviour, not docs): returns -1, errno EINVAL, handler called once, file pointer and buffer untouched.
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

#define N "tc_io_neg_read_count_toobig_state_probe_" TC_CRT_DLL ".tmp"

static int op(void)
{
    int fd;
    rm(N); mk(N, "0123456789", 10);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    return fd;
}
TC_CASE(read_uint_max)
{
    int fd = op(), r; char buf[16]; long pos;
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    _lseek(fd, 3, SEEK_SET);
    memset(buf, 0x5a, sizeof buf);
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, UINT_MAX);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    pos = _tell(fd); CHECK_EQ_I(pos, 3);
    CHECK_EQ_I((unsigned char)buf[0], 0x5a); CHECK_EQ_I((unsigned char)buf[15], 0x5a);
    _close(fd); rm(N);
}
TC_CASE(read_int_max_plus_1)
{
    int fd = op(), r; char buf[16];
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    memset(buf, 0x5a, sizeof buf);
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, (unsigned)INT_MAX + 1u);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_tell(fd), 0);
    CHECK_EQ_I((unsigned char)buf[0], 0x5a);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_uint_max); TC_RUN(read_int_max_plus_1);
    return tc_end();
}
