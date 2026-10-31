/* TC
 * id:    gtm_neg_utime_missing_file
 * area:  time
 * funcs: _utime32 _wutime32
 * procs: _utime _wutime
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _utime: returns -1 and sets errno (ENOENT for missing file; NULL filename -> invalid parameter handler, EINVAL)
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
typedef int (__cdecl *wutime_t)(const wchar_t *, ubuf *);
static utime_t p_utime; static wutime_t p_wutime;
static ubuf B = { 1000000000, 1100000000 };
static struct __utimbuf32 B32 = { 1000000000, 1100000000 };
static const char *MISSING = "tc_gtm_neg_utime_missing_file_nonexistent.tmp";
static const wchar_t *WMISSING = L"tc_gtm_neg_utime_missing_file_nonexistent.tmp";
static const char *volatile np = NULL;
static const wchar_t *volatile wnp = NULL;

TC_CASE(utime_missing)
{
    CHECK(p_utime != NULL); if (!p_utime) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_utime(MISSING, &B), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wutime_missing)
{
    CHECK(p_wutime != NULL); if (!p_wutime) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_wutime(WMISSING, &B), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(sized32_missing)
{
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_utime32(MISSING, &B32), -1);
    CHECK_EQ_I(errno, ENOENT);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_wutime32(WMISSING, &B32), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(utime_null_path)
{
    CHECK(p_utime != NULL); if (!p_utime) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_utime(np, &B), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wutime_null_path)
{
    CHECK(p_wutime != NULL); if (!p_wutime) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(p_wutime(wnp, &B), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); p_utime = (utime_t)crt_or("_utime", "_utime" TC_SZ); p_wutime = (wutime_t)crt_or("_wutime", "_wutime" TC_SZ);
    TC_RUN(utime_missing); TC_RUN(wutime_missing); TC_RUN(sized32_missing); TC_RUN(utime_null_path); TC_RUN(wutime_null_path);
    return tc_end();
}
