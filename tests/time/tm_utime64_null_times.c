/* TC
 * id:    tm_utime64_null_times
 * area:  time
 * funcs: _utime64 _time64
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _utime: if times is NULL, the modification time is set to the current time
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
static void make_file(const char *path)
{
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}

static const char *F = "tc_tm_utime64_null_times_a.tmp";

TC_CASE(null_means_now)
{
    struct __utimbuf64 b; long long a = 0, m = 0, now;
    make_file(F);
    b.actime = 1000000000; b.modtime = 1000000000;
    CHECK_EQ_I(_utime64(F, &b), 0);
    CHECK_EQ_I(_utime64(F, NULL), 0);
    now = _time64(NULL);
    CHECK(get_times(F, &a, &m));
    CHECK(m >= now - 10 && m <= now + 10);
}
int main(void) { tc_begin(); TC_RUN(null_means_now); DeleteFileA(F); return tc_end(); }
