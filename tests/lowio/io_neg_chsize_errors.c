/* TC
 * id:    io_neg_chsize_errors
 * area:  lowio
 * funcs: _chsize _chsize_s _open _close
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _chsize / _chsize_s: errno EBADF, EINVAL; invalid parameter handler invoked for invalid fd/size
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
#define N "tc_io_neg_chsize_errors_1.tmp"
TC_CASE(negative_size)
{
    int fd; rm(N);
    tc_iph_install();   /* default handler is fatal on ucrt/msvcr80+ */
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    errno = 0; CHECK_EQ_I(_chsize(fd, -1), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_chsize_s(fd, -1), EINVAL);
    CHECK_EQ_I(_filelength(fd), 0);
    _close(fd); rm(N);
}
TC_CASE(bad_fd)
{
    int fd; tc_iph_install();
    errno = 0; CHECK_EQ_I(_chsize(-1, 1), -1); CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(_chsize_s(-1, 1), EBADF);
    mk(N, "a", 1);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    _close(fd);
    errno = 0; CHECK_EQ_I(_chsize(fd, 1), -1); CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(_chsize_s(fd, 1), EBADF);
    rm(N);
}
TC_CASE(iph_counted)
{
    int before;
    if (!tc_iph_install()) TC_SKIP("no _set_invalid_parameter_handler");
    before = tc_iph_count;
    _chsize_s(-1, 1);
    CHECK(tc_iph_count > before);
}
int main(void) { tc_begin(); TC_RUN(negative_size); TC_RUN(bad_fd); TC_RUN(iph_counted); return tc_end(); }
