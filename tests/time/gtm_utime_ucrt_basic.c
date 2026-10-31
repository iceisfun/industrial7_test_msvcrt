/* TC
 * id:    gtm_utime_ucrt_basic
 * area:  time
 * funcs: _open _close
 * procs: _utime
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _utime, _utime32, _utime64 (_utime maps to the 64-bit version unless _USE_32BIT_TIME_T)
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

typedef int (__cdecl *utime_t)(const char *, ubuf *);
static utime_t p_utime;
static const char *F = "tc_gtm_utime_ucrt_basic_a.tmp";
static void make(void) { int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); if (fd >= 0) _close(fd); }

TC_CASE(set_mtime)
{
    ubuf b; long long a = 0, m = 0;
    CHECK(p_utime != NULL); if (!p_utime) return;
    make(); b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(p_utime(F, &b), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(m, 1100000000);
}
TC_CASE(set_atime)
{
    ubuf b; long long a = 0, m = 0;
    CHECK(p_utime != NULL); if (!p_utime) return;
    make(); b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(p_utime(F, &b), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(a, 1000000000);
}
TC_CASE(null_is_now)
{
    ubuf b; long long a = 0, m = 0, now;
    CHECK(p_utime != NULL); if (!p_utime) return;
    make(); b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(p_utime(F, &b), 0);
    CHECK_EQ_I(p_utime(F, NULL), 0);
    now = now_unix();
    CHECK(get_times(F, &a, &m));
    CHECK(m >= now - 5 && m <= now + 5);
}
TC_CASE(post_2038)
{
    ubuf b; long long a = 0, m = 0;
    CHECK(p_utime != NULL); if (!p_utime) return;
    make(); b.actime = 4102444800LL; b.modtime = 4102444800LL;  /* 2100-01-01T00:00:00Z */
    CHECK_EQ_I(p_utime(F, &b), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(m, 4102444800LL);
}
int main(void)
{
    tc_begin(); p_utime = (utime_t)crt_or("_utime", "_utime" TC_SZ);
    TC_RUN(set_mtime); TC_RUN(set_atime); TC_RUN(null_is_now); TC_RUN(post_2038);
    DeleteFileA(F); return tc_end();
}
