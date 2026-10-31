/* TC
 * id:    io_neg_sopen_badshare
 * area:  lowio
 * funcs: _sopen _wsopen _open _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _sopen/_wsopen: NULL filename is an invalid parameter (EINVAL); missing file ENOENT; sharing violation reports EACCES
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
#define N "tc_io_neg_sopen_badshare_1.tmp"
TC_CASE(null_path)
{
    int c = tc_iph_count; const char *volatile p = NULL;
    errno = 0; CHECK_EQ_I(_sopen(p, _O_RDONLY, _SH_DENYNO), -1);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c + 1);
}
TC_CASE(wnull_path)
{
    int c = tc_iph_count; const wchar_t *volatile p = NULL;
    errno = 0; CHECK_EQ_I(_wsopen(p, _O_RDONLY, _SH_DENYNO), -1);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c + 1);
}
TC_CASE(missing)
{
    int c = tc_iph_count; const char *volatile p = "tc_io_neg_sopen_badshare_missing.tmp";
    rm(p);
    errno = 0; CHECK_EQ_I(_sopen(p, _O_RDONLY, _SH_DENYNO), -1);
    CHECK_EQ_I(errno, ENOENT); CHECK_EQ_I(tc_iph_count, c);
}
TC_CASE(deny_rw_conflict)
{
    int a, b, c = tc_iph_count; const char *volatile p = N;
    rm(N);
    a = _sopen(p, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, _S_IREAD | _S_IWRITE);
    CHECK(a >= 3); if (a < 0) return;
    errno = 0; b = _sopen(p, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1); CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    _close(a); rm(N);
    (void)c;
}
TC_CASE(no_fd_leak)
{
    int f0, f1; const char *volatile m = "tc_io_neg_sopen_badshare_missing.tmp";
    f0 = _open("NUL", _O_RDWR); CHECK(f0 >= 3); if (f0 < 0) return; _close(f0);
    _sopen(m, _O_RDONLY, _SH_DENYNO);
    { const char *volatile np = NULL; _sopen(np, _O_RDONLY, _SH_DENYNO); }
    f1 = _open("NUL", _O_RDWR); CHECK_EQ_I(f1, f0);
    if (f1 >= 0) _close(f1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_path); TC_RUN(wnull_path); TC_RUN(missing); TC_RUN(deny_rw_conflict); TC_RUN(no_fd_leak);
    rm(N); return tc_end();
}
