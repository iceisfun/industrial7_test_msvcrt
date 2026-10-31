/* TC
 * id:    tm_neg_utime64_probe_pre1970
 * area:  time
 * funcs: _utime64
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _utime (valid range ends 23:59:59 Dec 31 3000 UTC = 32535215999)
 * note:  PROBES whether _utime64 accepts out-of-range timestamps (negative, beyond year 3000, huge).
 *        Only asserts: rc is 0 or -1; on -1 errno is EINVAL.
 */
#include "tc.h"
#include <sys/utime.h>
#include <time.h>
#include <errno.h>
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

static const char *F = "tc_tm_neg_utime64_probe_pre1970_a.tmp";

static void probe(long long at, long long mt)
{
    struct __utimbuf64 b, ok; int rc;
    CHECK(tc_iph_install());
    make_file(F);
    ok.actime = 1000000000; ok.modtime = 1100000000;
    CHECK_EQ_I(_utime64(F, &ok), 0);
    b.actime = at; b.modtime = mt;
    errno = 0;
    rc = _utime64(F, &b);
    CHECK(rc == 0 || rc == -1);
    if (rc == -1) {
        CHECK_EQ_I(errno, EINVAL);
    }
}
TC_CASE(minus_one) { CHECK(tc_iph_install()); probe(-1, -1); }
TC_CASE(beyond_3000) { probe(32535244800LL, 32535244800LL); }
TC_CASE(very_large) { probe(1LL << 40, 1LL << 40); }
int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(beyond_3000); TC_RUN(very_large); DeleteFileA(F); return tc_end(); }
