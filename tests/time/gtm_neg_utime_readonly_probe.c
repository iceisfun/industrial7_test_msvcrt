/* TC
 * id:    gtm_neg_utime_readonly_probe
 * area:  time
 * funcs: _open _close
 * procs: _utime _utime64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _utime
 * note:  PROBE: whether the read-only attribute blocks _utime. Either outcome is accepted (0 with the new mtime verified through
 *        GetFileTime, or -1 with errno EACCES); the result is printed as a TC NOTE line so real-Windows runs record which one occurs.
 *        The attribute is cleared before the file is deleted.
 */
#include "tc.h"
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct { long long actime, modtime; } ubuf;
typedef int (__cdecl *utime_t)(const char *, ubuf *);
static const char *F = "tc_gtm_neg_utime_readonly_probe_a.tmp";

static long long mtime_of(const char *path)
{
    FILETIME c, a, w; ULARGE_INTEGER u; long long r = -1;
    HANDLE h = CreateFileA(path, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (GetFileTime(h, &c, &a, &w)) { u.LowPart = w.dwLowDateTime; u.HighPart = w.dwHighDateTime; r = (long long)(u.QuadPart / 10000000ULL) - 11644473600LL; }
    CloseHandle(h); return r;
}
TC_CASE(readonly_probe)
{
    utime_t p = (utime_t)tc_crt_proc("_utime"); if (!p) p = (utime_t)tc_crt_proc("_utime64");
    int fd, r; ubuf b; long long m; char t[24];
    if (!p) TC_SKIP("no _utime");
    fd = _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); CHECK(fd >= 0); if (fd < 0) return;
    _close(fd);
    CHECK(SetFileAttributesA(F, FILE_ATTRIBUTE_READONLY));
    b.actime = 1000000000; b.modtime = 1100000000;
    errno = 0; r = p(F, &b);
    tc_write("TC NOTE _utime on read-only file ret="); tc_i64(r, t); tc_write(t); tc_write(" errno="); tc_i64(errno, t); tc_write(t); tc_write("\n");
    CHECK(r == 0 || r == -1);
    if (r == 0) { m = mtime_of(F); CHECK_EQ_I(m, 1100000000); }
    else CHECK_EQ_I(errno, EACCES);
    SetFileAttributesA(F, FILE_ATTRIBUTE_NORMAL); DeleteFileA(F);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(readonly_probe);
    SetFileAttributesA(F, FILE_ATTRIBUTE_NORMAL); DeleteFileA(F);
    return tc_end();
}
