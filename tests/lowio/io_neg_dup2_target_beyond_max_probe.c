/* TC
 * id:    io_neg_dup2_target_beyond_max_probe
 * area:  lowio
 * funcs: _dup2 _open _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmaxstdio (hard cap 8192 descriptors), _dup2
 * note:  Probes _dup2 to target 8192 (first index past the 8192-entry table; asserted to fail) and 2048 (inside the table; outcome only recorded as TC INFO, errno recorded).
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
#define N "tc_io_neg_dup2_target_beyond_max_probe_1.tmp"
TC_CASE(beyond)
{
    int fd, r1, r2, e1, e2; volatile int t1 = 8192, t2 = 2048;
    rm(N); fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3); if (fd < 0) return;
    errno = 0; r1 = _dup2(fd, t1); e1 = errno;
    errno = 0; r2 = _dup2(fd, t2); e2 = errno;
    note("rc_8192", r1); note("errno_8192", e1); note("rc_2048", r2); note("errno_2048", e2);
    CHECK_EQ_I(r1, -1);
    if (r1 == 0) _close(t1);
    if (r2 == 0) _close(t2);
    _close(fd);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(beyond); rm(N); return tc_end(); }
