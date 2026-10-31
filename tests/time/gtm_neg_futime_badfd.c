/* TC
 * id:    gtm_neg_futime_badfd
 * area:  time
 * funcs: _futime32 _open _close
 * procs: _futime
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _futime: invalid descriptor -> invalid parameter handler, errno EBADF, returns -1
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

static long long ft_unix(const FILETIME *f)
{
    ULARGE_INTEGER u; u.LowPart = f->dwLowDateTime; u.HighPart = f->dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}
static long long now_unix(void)
{
    FILETIME f; GetSystemTimeAsFileTime(&f); return ft_unix(&f);
}
static int get_times(const char *path, long long *a, long long *m)
{
    FILETIME ca, aa, wa; int ok;
    HANDLE h = CreateFileA(path, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    ok = GetFileTime(h, &ca, &aa, &wa);
    CloseHandle(h);
    if (ok) { *a = ft_unix(&aa); *m = ft_unix(&wa); }
    return ok;
}
static int get_timesw(const wchar_t *path, long long *a, long long *m)
{
    FILETIME ca, aa, wa; int ok;
    HANDLE h = CreateFileW(path, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    ok = GetFileTime(h, &ca, &aa, &wa);
    CloseHandle(h);
    if (ok) { *a = ft_unix(&aa); *m = ft_unix(&wa); }
    return ok;
}
/* Pre-UCRT CRTs may convert time_t through local time with the current DST bias; tolerate a
 * +/-1h skew there (timezone dependent). UCRT must be exact. */
static long long norm(long long g, long long w)
{
    if (TC_BELOW(140) && (g - w == 3600 || g - w == -3600)) return w;
    return g;
}
/* plain _utime/_futime/_wutime take a 64-bit time_t buffer in UCRT and on x64; 32-bit on x86 Win7-era CRTs */
#if TC_X64 || TC_CRT_VER >= 140
typedef struct { long long actime, modtime; } ubuf;
#else
typedef struct { long actime, modtime; } ubuf;
#endif

typedef int (__cdecl *futime_t)(int, ubuf *);
static futime_t p_futime;
static ubuf B = { 1000000000, 1100000000 };
static struct __utimbuf32 B32 = { 1000000000, 1100000000 };

static const char *F = "tc_gtm_neg_futime_badfd_a.tmp";
static int closed_fd(void)
{
    int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
    return fd;
}

#define _futime_p p_futime
TC_CASE(futime_fd_minus1)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_futime(-1, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime_closed_fd)
{
    int fd = closed_fd();
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_futime(fd, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime_fd_9999)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_futime(9999, &B), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime32_fd_minus1)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(-1, &B32), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime32_closed_fd)
{
    int fd = closed_fd();
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(fd, &B32), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(futime32_fd_9999)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_futime32(9999, &B32), -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); p_futime = (futime_t)crt_or("_futime", "_futime" TC_SZ);
    TC_RUN(futime_fd_minus1); TC_RUN(futime_closed_fd); TC_RUN(futime_fd_9999);
    TC_RUN(futime32_fd_minus1); TC_RUN(futime32_closed_fd); TC_RUN(futime32_fd_9999);
    DeleteFileA(F); return tc_end();
}
