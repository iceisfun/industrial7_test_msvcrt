/* TC
 * id:    gtm_wutime32_basic
 * area:  time
 * funcs: _wutime32 _wopen _close
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _utime, _wutime32
 * note:  msvcrt.dll excluded: export unsure. Old CRTs tolerate a +/-1h local-time skew.
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

static const wchar_t *WF = L"tc_gtm_wutime32_basic_a.tmp";
static void make(void) { int fd = _wopen(WF, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); if (fd >= 0) _close(fd); }

TC_CASE(set_times)
{
    struct __utimbuf32 b; long long a = 0, m = 0;
    make(); b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(_wutime32(WF, &b), 0);
    CHECK(get_timesw(WF, &a, &m));
    CHECK_EQ_I(norm(a, 1000000000), 1000000000);
    CHECK_EQ_I(norm(m, 1100000000), 1100000000);
}
TC_CASE(max_int32)
{
    struct __utimbuf32 b; long long a = 0, m = 0;
    /* MS docs: the 32-bit range ends at 23:59:59 Jan 18 2038 UTC, so 0x7fffffff (Jan 19 03:14:07) is outside it and old CRTs
     * convert via localtime32; use a value ~36h below INT32_MAX, safely inside the documented range in every time zone. */
    make(); b.actime = 0x7ffe0000; b.modtime = 0x7ffe0000;
    CHECK_EQ_I(_wutime32(WF, &b), 0);
    CHECK(get_timesw(WF, &a, &m));
    CHECK_EQ_I(norm(m, 0x7ffe0000LL), 0x7ffe0000LL);
    CHECK_EQ_I(norm(a, 0x7ffe0000LL), 0x7ffe0000LL);
}
TC_CASE(null_is_now)
{
    struct __utimbuf32 b; long long a = 0, m = 0, now;
    make(); b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(_wutime32(WF, &b), 0);
    CHECK_EQ_I(_wutime32(WF, NULL), 0);
    now = now_unix();
    CHECK(get_timesw(WF, &a, &m));
    CHECK(m >= now - 5 && m <= now + 5);
}
int main(void)
{
    tc_begin(); TC_RUN(set_times); TC_RUN(max_int32); TC_RUN(null_is_now);
    DeleteFileW(WF); return tc_end();
}
