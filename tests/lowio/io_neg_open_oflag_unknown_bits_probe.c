/* TC
 * id:    io_neg_open_oflag_unknown_bits_probe
 * area:  lowio
 * funcs: _open _sopen_s _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: oflag constants; invalid oflag -> EINVAL
 * note:  PROBE: (a) an unknown high oflag bit 0x40000000, (b) access-mode bits _O_WRONLY|_O_RDWR (value 3), (c) _O_APPEND|_O_RDONLY. Whether each is rejected is not documented; asserts only invariants: failure is -1/EINVAL with handler count 0 or 1, _sopen_s agrees with _open and sets *pfh=-1 on failure. Outcome recorded.
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

#define N "tc_io_neg_open_oflag_unknown_bits_probe_" TC_CRT_DLL ".tmp"
#include <share.h>

static void probe(const char *k, int oflag)
{
    int fd, e, fd2 = 777, r, c1, c2;
    CHECK(tc_iph_install());
    rm(N); tc_iph_count = 0; errno = 0;
    fd = _open(N, oflag | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    e = errno; c1 = tc_iph_count;
    note(k, fd);
    if (fd >= 0) _close(fd);
    rm(N);
    tc_iph_count = 0;
    r = _sopen_s(&fd2, N, oflag | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    c2 = tc_iph_count;
    if (fd2 >= 0) _close(fd2);
    rm(N);
    if (fd < 0) {
        CHECK_EQ_I(fd, -1); CHECK_EQ_I(e, EINVAL); CHECK(c1 <= 1);
        CHECK(r != 0); CHECK_EQ_I(fd2, -1); CHECK(c2 <= 1);
    } else {
        CHECK_EQ_I(r, 0);
    }
}
TC_CASE(unknown_high_bit) { probe("unknown_bit", 0x40000000 | _O_WRONLY); }
TC_CASE(wronly_and_rdwr)  { probe("wronly_rdwr", _O_WRONLY | _O_RDWR); }
TC_CASE(append_rdonly)    { probe("append_rdonly", _O_APPEND | _O_RDONLY); }
int main(void)
{
    tc_begin();
    TC_RUN(unknown_high_bit); TC_RUN(wronly_and_rdwr); TC_RUN(append_rdonly);
    return tc_end();
}
