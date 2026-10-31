/* TC
 * id:    gtm_futime_old_basic
 * area:  time
 * funcs: _open _close
 * procs: _futime _utime _wutime
 * crt:   msvcr80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _utime, _futime (plain names, time_t-sized buffer)
 * note:  msvcrt.dll deliberately left out pending a probe of export names. Old CRTs tolerate a +/-1h local-time skew.
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
typedef int (__cdecl *utime_t)(const char *, ubuf *);
typedef int (__cdecl *wutime_t)(const wchar_t *, ubuf *);
static futime_t p_futime; static utime_t p_utime; static wutime_t p_wutime;
static const char *F = "tc_gtm_futime_old_basic_a.tmp";
static const wchar_t *WF = L"tc_gtm_futime_old_basic_a.tmp";
static void make(void) { int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); if (fd >= 0) _close(fd); }

TC_CASE(futime_fd)
{
    ubuf b; long long a = 0, m = 0; int fd;
    CHECK(p_futime != NULL); if (!p_futime) return;
    fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(p_futime(fd, &b), 0);
    CHECK_EQ_I(_close(fd), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(norm(a, 1000000000), 1000000000);
    CHECK_EQ_I(norm(m, 1100000000), 1100000000);
}
TC_CASE(utime_path)
{
    ubuf b; long long a = 0, m = 0;
    CHECK(p_utime != NULL); if (!p_utime) return;
    make(); b.actime = 1000000100; b.modtime = 1100000100;
    CHECK_EQ_I(p_utime(F, &b), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(norm(a, 1000000100), 1000000100);
    CHECK_EQ_I(norm(m, 1100000100), 1100000100);
}
TC_CASE(wutime_path)
{
    ubuf b; long long a = 0, m = 0;
    CHECK(p_wutime != NULL); if (!p_wutime) return;
    make(); b.actime = 1000000200; b.modtime = 1100000200;
    CHECK_EQ_I(p_wutime(WF, &b), 0);
    CHECK(get_timesw(WF, &a, &m));
    CHECK_EQ_I(norm(a, 1000000200), 1000000200);
    CHECK_EQ_I(norm(m, 1100000200), 1100000200);
}
int main(void)
{
    tc_begin();
    p_futime = (futime_t)crt_or("_futime", "_futime" TC_SZ); p_utime = (utime_t)crt_or("_utime", "_utime" TC_SZ); p_wutime = (wutime_t)crt_or("_wutime", "_wutime" TC_SZ);
    TC_RUN(futime_fd); TC_RUN(utime_path); TC_RUN(wutime_path);
    DeleteFileA(F); return tc_end();
}
