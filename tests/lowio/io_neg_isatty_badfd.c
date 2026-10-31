/* TC
 * id:    io_neg_isatty_badfd
 * area:  lowio
 * funcs: _isatty _open _close _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _isatty: returns 0 if not a character device
 * note:  Only the return value (0) is asserted; errno/handler behaviour is in io_neg_isatty_badfd_errno. A handler is installed because the newer CRTs treat a bad fd as an invalid parameter (fatal by default).
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

#define N "tc_io_neg_isatty_badfd_" TC_CRT_DLL ".tmp"

TC_CASE(minus_one) { CHECK(tc_iph_install()); CHECK_EQ_I(_isatty(-1), 0); }
TC_CASE(huge) { CHECK(tc_iph_install()); CHECK_EQ_I(_isatty(0x7FFFFFF0), 0); }
TC_CASE(closed)
{
    int fd; CHECK(tc_iph_install());
    mk(N, "x", 1);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 3);
    if (fd >= 0) { _close(fd); CHECK_EQ_I(_isatty(fd), 0); }
    rm(N);
}

int main(void)
{
    tc_begin();
    TC_RUN(minus_one);
    TC_RUN(huge);
    TC_RUN(closed);
    return tc_end();
}
