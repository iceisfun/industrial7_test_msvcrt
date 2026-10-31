/* TC
 * id:    tm_wutime64_basic
 * area:  time
 * funcs: _wutime64
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _utime, _wutime64
 * note:  Old CRTs may skew by +/-1h (local-time conversion); tolerated for crt<140 only.
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
static long long ft_unix(const FILETIME *f)
{
    ULARGE_INTEGER u; u.LowPart = f->dwLowDateTime; u.HighPart = f->dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}
static int get_times(const char *path, long long *a, long long *m)
{
    FILETIME ca, aa, wa; HANDLE h = CreateFileA(path, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int ok;
    if (h == INVALID_HANDLE_VALUE) return 0;
    ok = GetFileTime(h, &ca, &aa, &wa);
    CloseHandle(h);
    if (ok) { *a = ft_unix(&aa); *m = ft_unix(&wa); }
    return ok;
}
/* Pre-UCRT CRTs may convert time_t through local time with the *current* DST bias; tolerate a
 * +/-1h skew there (timezone dependent). UCRT must be exact. */
static long long norm(long long g, long long w)
{
    if (TC_BELOW(140) && (g - w == 3600 || g - w == -3600)) return w;
    return g;
}
static void make_file(const char *path)
{
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}

static const char *F = "tc_tm_wutime64_basic_a.tmp";
static const wchar_t *WF = L"tc_tm_wutime64_basic_a.tmp";

TC_CASE(wide_set_and_read)
{
    struct __utimbuf64 b; long long a = 0, m = 0;
    make_file(F);
    b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(_wutime64(WF, &b), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(norm(a, 1000000000), 1000000000);
    CHECK_EQ_I(norm(m, 1100000000), 1100000000);
}
int main(void) { tc_begin(); TC_RUN(wide_set_and_read); DeleteFileA(F); return tc_end(); }
