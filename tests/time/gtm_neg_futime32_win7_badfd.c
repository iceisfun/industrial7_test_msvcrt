/* TC
 * id:    gtm_neg_futime32_win7_badfd
 * area:  time
 * funcs: _futime32 _open _close
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _futime32: invalid fd -> invalid parameter handler (msvcr80+), errno EBADF, returns -1
 */
#include "tc.h"
#include <sys/utime.h>
#include <sys/timeb.h>
#include <time.h>
#include <errno.h>
#include <io.h>
#include <wchar.h>
#include <fcntl.h>
#include <sys/stat.h>
#if TC_X64 || TC_CRT_VER >= 140
#define TC_SZ "64"
#else
#define TC_SZ "32"
#endif
/* plain name if exported, else the sized equivalent (same semantics where time_t is 64-bit / x86 32-bit) */
static FARPROC crt_or(const char *a, const char *b) { FARPROC p = tc_crt_proc(a); return p ? p : tc_crt_proc(b); }

static const char *F = "tc_gtm_neg_futime32_win7_badfd_a.tmp";
static int closed_fd(void)
{
    int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
    return fd;
}

static struct __utimbuf32 B = { 1000000000, 1100000000 };
TC_CASE(futime32_fd_minus1)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(-1, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime32_closed_fd)
{
    int fd = closed_fd();
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(fd, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime32_fd_9999)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(9999, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin();
    TC_RUN(futime32_fd_minus1); TC_RUN(futime32_closed_fd); TC_RUN(futime32_fd_9999);
    DeleteFileA(F); return tc_end();
}
