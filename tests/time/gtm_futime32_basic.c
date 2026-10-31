/* TC
 * id:    gtm_futime32_basic
 * area:  time
 * funcs: _futime32 _open _close
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _futime, _futime32
 * note:  Old CRTs tolerate a +/-1h local-time skew. Read-only fd behaviour is not covered here.
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

static const char *F = "tc_gtm_futime32_basic_a.tmp";

TC_CASE(set_by_fd)
{
    struct __utimbuf32 b; long long a = 0, m = 0; int fd;
    fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(_futime32(fd, &b), 0);
    CHECK_EQ_I(_close(fd), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(norm(a, 1000000000), 1000000000);
    CHECK_EQ_I(norm(m, 1100000000), 1100000000);
}
TC_CASE(null_is_now)
{
    long long a = 0, m = 0, now; int fd;
    fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    { struct __utimbuf32 o; o.actime = 1000000000; o.modtime = 1100000000; CHECK_EQ_I(_futime32(fd, &o), 0); } /* make 'now' distinguishable */
    CHECK_EQ_I(_futime32(fd, NULL), 0);
    CHECK_EQ_I(_close(fd), 0);
    now = now_unix();
    CHECK(get_times(F, &a, &m));
    CHECK(m >= now - 5 && m <= now + 5);
}
int main(void) { tc_begin(); TC_RUN(set_by_fd); TC_RUN(null_is_now); DeleteFileA(F); return tc_end(); }
