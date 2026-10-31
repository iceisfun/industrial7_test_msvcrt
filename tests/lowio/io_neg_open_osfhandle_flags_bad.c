/* TC
 * id:    io_neg_open_osfhandle_flags_bad
 * area:  lowio
 * funcs: _open_osfhandle _get_osfhandle _close
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _get_osfhandle: invalid fd -> invalid parameter handler, EBADF, returns INVALID_HANDLE_VALUE (-1)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>

static char N[] = "tc_io_neg_open_osfhandle_flags_bad.tmp";

static HANDLE mk(void)
{
    return CreateFileA(N, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}
TC_CASE(get_after_close)
{
    HANDLE h = mk(); int fd;
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h == INVALID_HANDLE_VALUE) return;
    fd = _open_osfhandle((intptr_t)h, _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) { CloseHandle(h); DeleteFileA(N); return; }
    CHECK_EQ_I(_close(fd), 0);
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_get_osfhandle(fd), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
    DeleteFileA(N);
}
TC_CASE(get_negative_and_huge)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_get_osfhandle(-1), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_get_osfhandle(100000), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(get_after_close); TC_RUN(get_negative_and_huge); return tc_end(); }
