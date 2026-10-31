/* TC
 * id:    io_neg_setmode_args
 * area:  lowio
 * funcs: _setmode _open _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmode: invalid fd or mode -> EBADF / EINVAL and invalid parameter handler; returns -1
 * note:  Mode must be exactly one of _O_TEXT, _O_BINARY, _O_WTEXT, _O_U16TEXT, _O_U8TEXT; zero, negative and TEXT|BINARY are invalid and leave the fd's mode unchanged.
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

#define N "tc_io_neg_setmode_args_" TC_CRT_DLL ".tmp"

static int openbin(void)
{
    int fd;
    rm(N); mk(N, "x", 1);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    return fd;
}
TC_CASE(bad_fd_minus1)
{
    int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _setmode(-1, _O_TEXT);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(closed_fd)
{
    int fd = openbin(), r;
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    _close(fd);
    tc_iph_count = 0; errno = 0;
    r = _setmode(fd, _O_BINARY);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 1);
    rm(N);
}
static void badmode(int mode)
{
    int fd = openbin(), r;
    CHECK(tc_iph_install());
    if (fd < 0) { rm(N); return; }
    tc_iph_count = 0; errno = 0;
    r = _setmode(fd, mode);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = _setmode(fd, _O_TEXT);
    CHECK_EQ_I(r, _O_BINARY);
    _close(fd); rm(N);
}
TC_CASE(mode_zero)     { badmode(0); }
TC_CASE(mode_minus1)   { badmode(-1); }
TC_CASE(mode_text_binary) { badmode(_O_TEXT | _O_BINARY); }
int main(void)
{
    tc_begin();
    TC_RUN(bad_fd_minus1); TC_RUN(closed_fd); TC_RUN(mode_zero); TC_RUN(mode_minus1); TC_RUN(mode_text_binary);
    return tc_end();
}
