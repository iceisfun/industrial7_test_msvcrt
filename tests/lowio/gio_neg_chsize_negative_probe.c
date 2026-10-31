/* TC
 * id:    gio_neg_chsize_negative_probe
 * area:  lowio
 * funcs: _chsize _chsize_s _open _write _filelength _close
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _chsize_s (EINVAL for negative size)
 * note:  PROBES negative target sizes: _chsize_s returns EINVAL, _chsize returns -1 with EINVAL, length unchanged.
 *        The INT64_MAX case was dropped on purpose: the CRT extends by writing zeros and could fill the disk.
 *        Invalid-parameter handler count is deliberately not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
#define N "tc_gio_neg_chsize_negative_probe.tmp"
static int mkf(void)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _write(fd, "abcde", 5);
    return fd;
}
TC_CASE(chsize_s_negative)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_chsize_s(fd, -5), EINVAL);
    CHECK_EQ_I(_filelength(fd), 5);
    _close(fd); rm(N);
}
TC_CASE(chsize_negative)
{
    int fd = mkf(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0;
    CHECK_EQ_I(_chsize(fd, -5), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_filelength(fd), 5);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(chsize_s_negative); TC_RUN(chsize_negative);
    rm(N);
    return tc_end();
}
