/* TC
 * id:    gtm_tzset_plain_alias_probe
 * area:  time
 * funcs: _tzset _putenv
 * procs: tzset utime _get_timezone
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs tzset, utime (deprecated POSIX names; oldnames.lib in the UCRT)
 * note:  PROBE: whether the DLL itself exports the non-underscore names tzset / utime. Absence is never asserted: the existence
 *        is printed as a TC NOTE line and the corresponding case is skipped when GetProcAddress fails. Where present, tzset must
 *        behave as _tzset and utime as _utime (x64 layout with 64-bit time_t).
 */
#include "tc.h"
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef void (__cdecl *tzset_t)(void);
typedef struct { long long actime, modtime; } ubuf;
typedef int (__cdecl *utime_t)(const char *, ubuf *);
typedef int (__cdecl *gettz_t)(long *);
static const char *F = "tc_gtm_tzset_plain_alias_probe_a.tmp";

TC_CASE(record_exports)
{
    tc_write("TC NOTE tzset exported: "); tc_write(tc_crt_proc("tzset") ? "yes\n" : "no\n");
    tc_write("TC NOTE utime exported: "); tc_write(tc_crt_proc("utime") ? "yes\n" : "no\n");
    CHECK(1);
}
TC_CASE(tzset_pst)
{
    tzset_t p = (tzset_t)tc_crt_proc("tzset"); gettz_t g = (gettz_t)tc_crt_proc("_get_timezone"); long tz = 0;
    if (!p) TC_SKIP("tzset not exported");
    if (!g) TC_SKIP("_get_timezone not exported");
    _putenv("TZ=PST8PDT"); p();
    CHECK_EQ_I(g(&tz), 0);
    CHECK_EQ_I(tz, 28800);
    _putenv("TZ="); _tzset();
}
TC_CASE(utime_plain)
{
    utime_t p = (utime_t)tc_crt_proc("utime"); ubuf b; int fd, r; long long m = -1;
    FILETIME c, a, w; HANDLE h;
    if (!p) TC_SKIP("utime not exported");
    fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); CHECK(fd >= 0); if (fd < 0) return;
    _close(fd);
    b.actime = 1000000000; b.modtime = 1100000000;
    r = p(F, &b);
    CHECK_EQ_I(r, 0);
    h = CreateFileA(F, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) {
        if (GetFileTime(h, &c, &a, &w)) { ULARGE_INTEGER u; u.LowPart = w.dwLowDateTime; u.HighPart = w.dwHighDateTime; m = (long long)(u.QuadPart / 10000000ULL) - 11644473600LL; }
        CloseHandle(h);
    }
    /* pre-UCRT CRTs convert through local time: tolerate a one-hour DST skew there */
    if (TC_BELOW(140) && (m - 1100000000 == 3600 || m - 1100000000 == -3600)) m = 1100000000;
    CHECK_EQ_I(m, 1100000000);
    DeleteFileA(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(record_exports); TC_RUN(tzset_pst); TC_RUN(utime_plain);
    DeleteFileA(F);
    return tc_end();
}
