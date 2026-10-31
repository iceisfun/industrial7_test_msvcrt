/* TC
 * id:    tm_futime64_basic
 * area:  time
 * funcs: _futime64 _open _close
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _futime, _futime64
 * note:  Old CRTs may skew by +/-1h (local-time conversion); tolerated for crt<140 only.
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
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

static const char *F = "tc_tm_futime64_basic_a.tmp";

TC_CASE(set_by_fd)
{
    struct __utimbuf64 b; long long a = 0, m = 0;
    int fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    b.actime = 1000000000; b.modtime = 1100000000;
    CHECK_EQ_I(_futime64(fd, &b), 0);
    CHECK_EQ_I(_close(fd), 0);
    CHECK(get_times(F, &a, &m));
    CHECK_EQ_I(norm(a, 1000000000), 1000000000);
    CHECK_EQ_I(norm(m, 1100000000), 1100000000);
}
TC_CASE(null_is_now)
{
    long long a = 0, m = 0, now;
    int fd = _open(F, _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_futime64(fd, NULL), 0);
    CHECK_EQ_I(_close(fd), 0);
    now = _time64(NULL);
    CHECK(get_times(F, &a, &m));
    CHECK(m >= now - 10 && m <= now + 10);
}
int main(void) { tc_begin(); TC_RUN(set_by_fd); TC_RUN(null_is_now); DeleteFileA(F); return tc_end(); }
