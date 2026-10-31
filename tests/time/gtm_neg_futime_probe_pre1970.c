/* TC
 * id:    gtm_neg_futime_probe_pre1970
 * area:  time
 * funcs: _open _close _unlink
 * procs: _futime _futime64 _futime32
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _futime, _utime ("... time_t values ... January 1, 1970 ... 23:59:59 December 31, 3000 UTC")
 * note:  PROBE range validation of _futime/_futime32: negative (pre-1970) times and times past 3000-12-31 23:59:59 UTC
 *        (32535215999) are expected to fail with -1/EINVAL. Year 3000-01-01 (32503680000) is inside the documented
 *        range and is probed separately (expected success). The invalid-parameter handler is installed, so a handler call is tolerated.
 */
#include "tc.h"
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef int (__cdecl *futime64_t)(int, void *);
typedef int (__cdecl *futime32_t)(int, void *);
static futime64_t p64; static futime32_t p32;
static const char *F = "tc_gtm_neg_futime_probe_pre1970_a.tmp";

static int mk(void) { return _open(F, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE); }

TC_CASE(exports) { CHECK(p64 != NULL); CHECK(p32 != NULL); }
TC_CASE(futime32_atime_minus1)
{
    int fd, r; int b[2] = { -1, 1000000000 };
    if (!p32) TC_SKIP("no _futime32");
    fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; r = p32(fd, b);
    { char t[24]; tc_i64(r, t); tc_write("TC NOTE futime32(atime=-1) ret="); tc_write(t); tc_i64(errno, t); tc_write(" errno="); tc_write(t); tc_write("\n"); }
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    _close(fd); _unlink(F);
}
TC_CASE(futime64_year3000_ok)
{
    int fd, r; long long b[2] = { 32503680000LL, 32503680000LL };
    if (!p64) TC_SKIP("no _futime");
    fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    r = p64(fd, b);
    CHECK_EQ_I(r, 0);
    _close(fd); _unlink(F);
}
TC_CASE(futime64_past_limit)
{
    int fd, r; long long b[2] = { 32535216000LL, 32535216000LL };  /* 3001-01-01T00:00:00Z */
    if (!p64) TC_SKIP("no _futime");
    fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; r = p64(fd, b);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    _close(fd); _unlink(F);
}
TC_CASE(futime64_negative)
{
    int fd, r; long long b[2] = { 1000000000LL, -86400LL };
    if (!p64) TC_SKIP("no _futime");
    fd = mk(); CHECK(fd >= 0); if (fd < 0) return;
    errno = 0; r = p64(fd, b);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    _close(fd); _unlink(F);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p64 = (futime64_t)tc_crt_proc("_futime"); if (!p64) p64 = (futime64_t)tc_crt_proc("_futime64"); p32 = (futime32_t)tc_crt_proc("_futime32");
    TC_RUN(exports); TC_RUN(futime32_atime_minus1); TC_RUN(futime64_year3000_ok);
    TC_RUN(futime64_past_limit); TC_RUN(futime64_negative);
    DeleteFileA(F);
    return tc_end();
}
