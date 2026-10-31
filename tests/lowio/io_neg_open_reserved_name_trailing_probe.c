/* TC
 * id:    io_neg_open_reserved_name_trailing_probe
 * area:  lowio
 * funcs: _open _close _isatty _filelength
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   Win32 path naming rules (CreateFile); MS docs _open
 * note:  PROBE: records (as TC NOTE lines) what _open/_creat do with the NUL device, 'nosuchdir\\x', a trailing-dot name, a trailing-space name, a \\?\ prefixed path and an NTFS alternate data stream name 'f:stream'. Only asserts there is no crash, that failures return exactly -1, and that successes return fd >= 3. Created files are removed.
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

#define N "tc_io_neg_open_reserved_name_trailing_probe_" TC_CRT_DLL ".tmp"

static void rec(const char *k, const char *path, int oflag)
{
    int fd, e;
    errno = 0;
    fd = _open(path, oflag, _S_IREAD | _S_IWRITE);
    e = errno;
    note(k, fd);
    if (fd < 0) { CHECK_EQ_I(fd, -1); note("errno", e); }
    else { CHECK(fd >= 3); _close(fd); }
}
TC_CASE(nul_device)
{
    int fd;
    CHECK(tc_iph_install());
    fd = _open("NUL", _O_RDWR | _O_BINARY);
    note("nul_fd", fd);
    if (fd >= 0) {
        note("nul_isatty", _isatty(fd));
        note("nul_filelength", _filelength(fd));
        _close(fd);
    }
}
TC_CASE(missing_dir)
{
    CHECK(tc_iph_install());
    rec("missing_dir_fd", "tc_io_nosuchdir\\x", _O_RDONLY);
    rec("missing_dir_slash_fd", "tc_io_nosuchdir/x", _O_RDONLY);
}
TC_CASE(trailing_dot)
{
    CHECK(tc_iph_install());
    rm("tc_io_rsv_dot");
    rec("trailing_dot_fd", "tc_io_rsv_dot.", _O_CREAT | _O_WRONLY | _O_BINARY);
    rm("tc_io_rsv_dot");
}
TC_CASE(trailing_space)
{
    CHECK(tc_iph_install());
    rm("tc_io_rsv_sp");
    rec("trailing_space_fd", "tc_io_rsv_sp ", _O_CREAT | _O_WRONLY | _O_BINARY);
    rm("tc_io_rsv_sp");
}
TC_CASE(verbatim_prefix)
{
    char cwd[MAX_PATH], p[MAX_PATH + 64]; int i, j = 0;
    CHECK(tc_iph_install());
    if (!GetCurrentDirectoryA(MAX_PATH, cwd)) return;
    p[j++]='\\'; p[j++]='\\'; p[j++]='?'; p[j++]='\\';
    for (i = 0; cwd[i]; i++) p[j++] = cwd[i];
    p[j++]='\\';
    { const char *t = "tc_io_rsv_verb.tmp"; for (i = 0; t[i]; i++) p[j++] = t[i]; }
    p[j] = 0;
    rm(p);
    rec("verbatim_fd", p, _O_CREAT | _O_WRONLY | _O_BINARY);
    rm(p);
}
TC_CASE(ads)
{
    CHECK(tc_iph_install());
    mk("tc_io_rsv_ads.tmp", "x", 1);
    rec("ads_fd", "tc_io_rsv_ads.tmp:stream", _O_CREAT | _O_WRONLY | _O_BINARY);
    rm("tc_io_rsv_ads.tmp");
}
int main(void)
{
    tc_begin();
    TC_RUN(nul_device); TC_RUN(missing_dir); TC_RUN(trailing_dot); TC_RUN(trailing_space);
    TC_RUN(verbatim_prefix); TC_RUN(ads);
    return tc_end();
}
