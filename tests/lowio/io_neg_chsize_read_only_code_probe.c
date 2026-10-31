/* TC
 * id:    io_neg_chsize_read_only_code_probe
 * area:  lowio
 * funcs: _chsize_s _open _close
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _chsize_s list EBADF for read-only file and EACCES for locked file
 * note:  Probes the exact error code from _chsize_s on an O_RDONLY fd: truncation (SetEndOfFile denied) is expected to give EACCES; extension (internal write denied) is guessed EACCES too (chsize maps ERROR_ACCESS_DENIED from the write to EACCES) but EBADF is the plausible alternative. Separate cases so each guess is isolated.
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
#define N "tc_io_neg_chsize_read_only_code_probe_1.tmp"
static int rdonly(void) { mk(N, "0123456789", 10); return _open(N, _O_RDONLY | _O_BINARY); }
TC_CASE(truncate_code)
{
    int fd = rdonly(), r;
    CHECK(fd >= 3); if (fd < 0) return;
    r = _chsize_s(fd, 3);
    note("truncate_rc", r);
    CHECK_EQ_I(r, EACCES);   /* SetEndOfFile denied */
    _close(fd); rm(N);
}
TC_CASE(extend_code)
{
    int fd = rdonly(), r;
    CHECK(fd >= 3); if (fd < 0) return;
    r = _chsize_s(fd, 50);
    note("extend_rc", r);
    CHECK_EQ_I(r, EACCES);   /* internal _write denied; the CRT maps ERROR_ACCESS_DENIED to EACCES (EBADF is the alternative) */
    _close(fd); rm(N);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(truncate_code); TC_RUN(extend_code); rm(N); return tc_end(); }
