/* TC
 * id:    io_neg_dup2_args
 * area:  lowio
 * funcs: _dup2 _open _close _write _lseek _read
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dup2: returns 0 on success, -1 and errno EBADF for an invalid descriptor (EMFILE if no more descriptors); invalid parameter handler invoked
 * note:  The out-of-range target (100000) asserts only -1 and errno in {EBADF, EMFILE}; see io_neg_dup2_target_beyond_max_probe for 8192/2048.
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
#define N "tc_io_neg_dup2_args_1.tmp"
static int ropen(void) { rm(N); return _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(target_minus1)
{
    int fd = ropen(), c = tc_iph_count; volatile int t = -1;
    CHECK(fd >= 3); if (fd < 0) return;
    errno = 0; CHECK_EQ_I(_dup2(fd, t), -1); CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, c + 1);
    CHECK_EQ_I(_write(fd, "a", 1), 1);
    _close(fd); rm(N);
}
TC_CASE(closed_source_keeps_target)
{
    int a = ropen(), b; volatile int bad; char x = 0;
    CHECK(a >= 3); if (a < 0) return;
    b = _open("NUL", _O_RDWR | _O_BINARY); CHECK(b >= 3); if (b < 0) { _close(a); return; }
    CHECK_EQ_I(_write(a, "q", 1), 1);
    _close(a); bad = a;
    errno = 0; CHECK_EQ_I(_dup2(bad, b), -1); CHECK_EQ_I(errno, EBADF);
    CHECK_EQ_I(_write(b, "z", 1), 1);   /* target still open */
    (void)x;
    _close(b); rm(N);
}
TC_CASE(failure_keeps_target_open)
{
    int src = ropen(), tgt; volatile int bad = -1; char buf[4] = {0};
    CHECK(src >= 3); if (src < 0) return;
    tgt = _open("tc_io_neg_dup2_args_2.tmp", _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(tgt >= 3); if (tgt < 0) { _close(src); return; }
    CHECK_EQ_I(_dup2(bad, tgt), -1);
    CHECK_EQ_I(_write(tgt, "hey", 3), 3);
    CHECK_EQ_I(_lseek(tgt, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(tgt, buf, 3), 3);
    CHECK_STR(buf, "hey");
    _close(src); _close(tgt); rm(N); rm("tc_io_neg_dup2_args_2.tmp");
}
TC_CASE(target_huge)
{
    int fd = ropen(); volatile int t = 100000; int r;
    CHECK(fd >= 3); if (fd < 0) return;
    errno = 0; r = _dup2(fd, t);
    CHECK_EQ_I(r, -1); CHECK(errno == EBADF || errno == EMFILE);
    if (r == 0) _close(t);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(target_minus1); TC_RUN(closed_source_keeps_target); TC_RUN(failure_keeps_target_open); TC_RUN(target_huge);
    rm(N); rm("tc_io_neg_dup2_args_2.tmp"); return tc_end();
}
