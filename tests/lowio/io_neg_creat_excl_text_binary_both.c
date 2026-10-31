/* TC
 * id:    io_neg_creat_excl_text_binary_both
 * area:  lowio
 * funcs: _open _wopen _sopen_s _close
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: _O_TEXT / _O_BINARY mutually exclusive translation modes
 * note:  PROBE: _O_TEXT|_O_BINARY together. Whether the UCRT rejects it with EINVAL (+handler) or lets one win silently is not documented. Asserts only the consistency invariants: _open/_wopen/_sopen_s agree on success vs failure; failure is -1 with errno==EINVAL; _sopen_s sets *pfh=-1 on failure; the handler count is 0 or 1 (never more). Records the outcome.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, (DWORD)cap, &r, NULL); CloseHandle(h); return (int)r;
}
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void note(const char *k, long long v)
{
    char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n");
}

#define N "tc_io_neg_creat_excl_text_binary_both_" TC_CRT_DLL ".tmp"
#include <share.h>

static int ok_open, ok_wopen, ok_sopen;
TC_CASE(open_both)
{
    int fd;
    CHECK(tc_iph_install());
    rm(N); tc_iph_count = 0; errno = 0;
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_TEXT | _O_BINARY, _S_IREAD | _S_IWRITE);
    note("rc", fd); note("errno", errno); note("iph", tc_iph_count);
    if (fd < 0) { CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count == 1); }
    else { CHECK(fd >= 3); CHECK_EQ_I(tc_iph_count, 0); _close(fd); ok_open = 1; }
    rm(N);
}
TC_CASE(wopen_both)
{
    int fd;
    CHECK(tc_iph_install());
    rm(N); tc_iph_count = 0; errno = 0;
    fd = _wopen(L"tc_io_neg_creat_excl_text_binary_both_w.tmp", _O_CREAT | _O_WRONLY | _O_TEXT | _O_BINARY, _S_IREAD | _S_IWRITE);
    note("rc", fd); note("iph", tc_iph_count);
    if (fd < 0) { CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count == 1); }
    else { CHECK(fd >= 3); _close(fd); ok_wopen = 1; }
    rm("tc_io_neg_creat_excl_text_binary_both_w.tmp");
}
TC_CASE(sopen_s_both)
{
    int fd = 12345, r;
    CHECK(tc_iph_install());
    rm(N); tc_iph_count = 0;
    r = _sopen_s(&fd, N, _O_CREAT | _O_WRONLY | _O_TEXT | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    note("ret", r); note("fd", fd); note("iph", tc_iph_count);
    if (r != 0) { CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fd, -1); CHECK(tc_iph_count == 1); }
    else { CHECK(fd >= 3); _close(fd); ok_sopen = 1; }
    rm(N);
}
TC_CASE(consistent)
{
    CHECK_EQ_I(ok_open, ok_wopen);
    CHECK_EQ_I(ok_open, ok_sopen);
}
int main(void)
{
    tc_begin();
    TC_RUN(open_both); TC_RUN(wopen_both); TC_RUN(sopen_s_both); TC_RUN(consistent);
    return tc_end();
}
