/* TC
 * id:    io_neg_sopen_shflag_invalid_probe
 * area:  lowio
 * funcs: _sopen _sopen_s _open _close
 * procs: _wsopen_s
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sopen: shflag must be one of _SH_DENYRW/_SH_DENYWR/_SH_DENYRD/_SH_DENYNO/_SH_SECURE
 * note:  Probes unknown shflag values (0x1234, -1): most likely rejected with EINVAL and the invalid parameter handler (asserted). rc/errno/iph recorded as TC INFO; no-crash and no-fd-leak asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <share.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static void note(const char *k, long long v) { char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }
typedef int (__cdecl *wsopen_s_t)(int *, const wchar_t *, int, int, int);
#define N "tc_io_neg_sopen_shflag_invalid_probe_1.tmp"
static int nextfd(void) { int f = _open("NUL", _O_RDWR); if (f >= 0) _close(f); return f; }
TC_CASE(sopen_bad_shflag)
{
    int f0 = nextfd(), r, e, c = tc_iph_count; volatile int sh = 0x1234;
    mk(N, "x", 1);
    errno = 0; r = _sopen(N, _O_RDONLY, sh); e = errno;
    note("sopen_rc", r); note("sopen_errno", e); note("sopen_iph", tc_iph_count - c);
    if (r >= 0) _close(r);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(e, EINVAL);
    CHECK_EQ_I(nextfd(), f0);
    sh = -1; errno = 0; r = _sopen(N, _O_RDONLY, sh);
    note("sopen_m1_rc", r); note("sopen_m1_errno", errno);
    if (r >= 0) _close(r);
    CHECK_EQ_I(nextfd(), f0);
    rm(N);
}
TC_CASE(sopen_s_bad_shflag)
{
    int f0 = nextfd(), fd = -7, r, c = tc_iph_count; volatile int sh = 0x1234;
    mk(N, "x", 1);
    errno = 0; r = _sopen_s(&fd, N, _O_RDONLY, sh, 0);
    note("sopen_s_rc", r); note("sopen_s_fd", fd); note("sopen_s_iph", tc_iph_count - c);
    if (r == 0 && fd >= 0) _close(fd);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(nextfd(), f0);
    rm(N);
}
TC_CASE(wsopen_s_bad_shflag)
{
    wsopen_s_t p = (wsopen_s_t)tc_crt_proc("_wsopen_s");
    static const wchar_t wn[] = L"tc_io_neg_sopen_shflag_invalid_probe_1.tmp";
    int f0 = nextfd(), fd = -7, r; volatile int sh = 0x1234;
    if (!p) TC_SKIP("_wsopen_s not exported");
    mk(N, "x", 1);
    r = p(&fd, wn, _O_RDONLY, sh, 0);
    note("wsopen_s_rc", r);
    if (r == 0 && fd >= 0) _close(fd);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(nextfd(), f0);
    rm(N);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(sopen_bad_shflag); TC_RUN(sopen_s_bad_shflag); TC_RUN(wsopen_s_bad_shflag);
    rm(N); return tc_end();
}
