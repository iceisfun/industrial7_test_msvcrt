/* TC
 * id:    io_neg_u16text_odd_bytes
 * area:  lowio
 * funcs: _open _write _read _close
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _write / _read: in Unicode mode count must be even; EINVAL
 * note:  Docs: if a file is opened in Unicode (UTF-16) mode, count must be even or errno = EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
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

#define N "tc_io_neg_u16text_odd_bytes_" TC_CRT_DLL ".tmp"

TC_CASE(write_odd)
{
    int fd, r; char buf[4] = { 'a', 0, 'b', 0 };
    CHECK(tc_iph_install());
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_U16TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    tc_iph_count = 0; errno = 0;
    r = _write(fd, buf, 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = _write(fd, buf, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    _close(fd); rm(N);
}
TC_CASE(read_odd)
{
    int fd, r; char buf[16]; static const char data[] = { (char)0xFF, (char)0xFE, 'a', 0, 'b', 0 };
    CHECK(tc_iph_install());
    mk(N, data, 6);
    fd = _open(N, _O_RDONLY | _O_U16TEXT);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    tc_iph_count = 0; errno = 0;
    r = _read(fd, buf, 3);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _close(fd); rm(N);
}
TC_CASE(write_even_ok)
{
    int fd; char buf[4] = { 'a', 0, 'b', 0 };
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_U16TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, buf, 4), 4);
    _close(fd); rm(N);
}

int main(void)
{
    tc_begin();
    TC_RUN(write_odd);
    TC_RUN(read_odd);
    TC_RUN(write_even_ok);
    return tc_end();
}
