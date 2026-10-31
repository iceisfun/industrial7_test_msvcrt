/* TC
 * id:    io_neg_setmode_invalid
 * area:  lowio
 * funcs: _setmode _open _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _setmode: EBADF invalid fd, EINVAL invalid mode; invalid parameter handler invoked
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#define N "tc_io_neg_setmode_invalid_1.tmp"
TC_CASE(bad_modes)
{
    int fd, before; rm(N);
    tc_iph_install();
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    before = tc_iph_count;
    errno = 0; CHECK_EQ_I(_setmode(fd, 0), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count > before);
    errno = 0; CHECK_EQ_I(_setmode(fd, _O_APPEND), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; CHECK_EQ_I(_setmode(fd, 0x12345), -1); CHECK_EQ_I(errno, EINVAL);
    /* mode must be unchanged: still binary */
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_BINARY);
    _close(fd); rm(N);
}
TC_CASE(bad_fd)
{
    int before;
    tc_iph_install();
    before = tc_iph_count;
    errno = 0; CHECK_EQ_I(_setmode(-1, _O_TEXT), -1); CHECK_EQ_I(errno, EBADF);
    CHECK(tc_iph_count > before);
}
TC_CASE(closed_fd)
{
    int fd; rm(N);
    tc_iph_install();
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    _close(fd);
    errno = 0; CHECK_EQ_I(_setmode(fd, _O_TEXT), -1); CHECK_EQ_I(errno, EBADF);
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(bad_modes); TC_RUN(bad_fd); TC_RUN(closed_fd); return tc_end(); }
