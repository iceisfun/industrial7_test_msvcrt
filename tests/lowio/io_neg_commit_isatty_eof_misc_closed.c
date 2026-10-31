/* TC
 * id:    io_neg_commit_isatty_eof_misc_closed
 * area:  lowio
 * funcs: _open _close _commit _eof _isatty _filelengthi64 _tell _lseeki64
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _commit, _eof, _filelengthi64, _tell, _lseeki64: invalid fd -> -1, EBADF and invalid parameter handler; _isatty returns 0
 * note:  Table-driven: every fd-taking function is called on a closed fd and on -1; each must return the failure value, set EBADF and call the invalid parameter handler exactly once (not for _isatty, whose errno/handler behaviour is covered by io_neg_isatty_badfd_errno). A freshly opened fd must work afterwards.
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

#define N "tc_io_neg_commit_isatty_eof_misc_closed_" TC_CRT_DLL ".tmp"

typedef long long (*fn_t)(int);
static long long f_commit(int fd)  { return _commit(fd); }
static long long f_eof(int fd)     { return _eof(fd); }
static long long f_flen64(int fd)  { return _filelengthi64(fd); }
static long long f_tell(int fd)    { return _tell(fd); }
static long long f_lseek64(int fd) { return _lseeki64(fd, 0, SEEK_CUR); }
static const struct { const char *name; fn_t fn; } tbl[] = {
    { "_commit", f_commit }, { "_eof", f_eof }, { "_filelengthi64", f_flen64 },
    { "_tell", f_tell }, { "_lseeki64", f_lseek64 },
};
static void run_all(int fd)
{
    size_t i;
    for (i = 0; i < sizeof tbl / sizeof tbl[0]; i++) {
        long long r; tc_iph_count = 0; errno = 0;
        r = tbl[i].fn(fd);
        if (r != -1) { tc_fail_expr(__FILE__, __LINE__, tbl[i].name); }
        if (errno != EBADF) { tc_fail_expr(__FILE__, __LINE__, tbl[i].name); }
        if (tc_iph_count != 1) { tc_fail_expr(__FILE__, __LINE__, tbl[i].name); }
    }
}
TC_CASE(closed_fd)
{
    int fd;
    CHECK(tc_iph_install());
    mk(N, "x", 1);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    _close(fd);
    run_all(fd);
    CHECK_EQ_I(_isatty(fd), 0);
    rm(N);
}
TC_CASE(minus_one)
{
    CHECK(tc_iph_install());
    run_all(-1);
    CHECK_EQ_I(_isatty(-1), 0);
}
TC_CASE(fresh_fd_works)
{
    int fd;
    CHECK(tc_iph_install());
    mk(N, "abc", 3);
    fd = _open(N, _O_RDWR | _O_BINARY);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    tc_iph_count = 0;
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I(_eof(fd), 0);
    CHECK_EQ_I(_filelengthi64(fd), 3);
    CHECK_EQ_I(_tell(fd), 0);
    CHECK_EQ_I(_lseeki64(fd, 0, SEEK_END), 3);
    CHECK_EQ_I(_eof(fd), 1);
    CHECK_EQ_I(_isatty(fd), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(closed_fd); TC_RUN(minus_one); TC_RUN(fresh_fd_works);
    return tc_end();
}
