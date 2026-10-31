/* TC
 * id:    gtm_ftime_basic
 * area:  time
 * funcs: 
 * procs: _ftime
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ftime, _ftime32, _ftime64
 * note:  Plain _ftime is resolved at run time (it is a macro/inline for the sized variants in the headers); if the DLL lacks the plain name the sized _ftime64/_ftime32 is used. Export presence is probed in gtm_ftime_probe_export.
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

#if TC_X64 || TC_CRT_VER >= 140
typedef struct __timeb64 TB;
#else
typedef struct __timeb32 TB;
#endif
typedef void (__cdecl *ftime_t)(TB *);
static ftime_t p_ftime;

static long long tb_ms(const TB *t) { return (long long)t->time * 1000 + t->millitm; }

TC_CASE(time_near_kernel32)
{
    TB t; long long ref;
    if (!p_ftime) TC_SKIP("_ftime not exported");
    ref = now_unix();
    p_ftime(&t);
    CHECK((long long)t.time >= ref - 5 && (long long)t.time <= ref + 5);
}
TC_CASE(millitm_range)
{
    TB t;
    if (!p_ftime) TC_SKIP("_ftime not exported");
    p_ftime(&t);
    CHECK(t.millitm < 1000);
}
TC_CASE(monotonic)
{
    TB a, b; long long d;
    if (!p_ftime) TC_SKIP("_ftime not exported");
    p_ftime(&a); p_ftime(&b);
    d = tb_ms(&b) - tb_ms(&a);
    CHECK(d >= 0);
    CHECK(d < 1000);
}
int main(void)
{
    tc_begin(); p_ftime = (ftime_t)crt_or("_ftime", "_ftime" TC_SZ);
    TC_RUN(time_near_kernel32); TC_RUN(millitm_range); TC_RUN(monotonic);
    return tc_end();
}
