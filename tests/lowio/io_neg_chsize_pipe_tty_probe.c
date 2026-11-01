/* TC
 * id:    io_neg_chsize_pipe_tty_probe
 * area:  lowio
 * funcs: _chsize_s _pipe _close _write _read
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _chsize_s (pipes are not seekable / have no size)
 * note:  Probes _chsize_s on the write end of an anonymous pipe: only rc != 0 and that the pipe is still usable are asserted; the errno-style rc is recorded as TC INFO.
 *        Win10 22H2 19045 (observed): _chsize_s(pipe_write_end, 1) returns 0 (no error) and the pipe stays usable (write 2 / read 2 succeed); the first two
 *        bytes read are recorded as TC INFO.
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
TC_CASE(pipe_write_end)
{
    int p[2], r; char b[4] = {0};
    if (_pipe(p, 4096, _O_BINARY) != 0) { CHECK(0); return; }
    r = _chsize_s(p[1], 1);
    note("rc", r);
    CHECK_EQ_I(r, 0);   /* observed: _chsize_s on a pipe reports success */
    CHECK_EQ_I(_write(p[1], "xy", 2), 2);
    CHECK_EQ_I(_read(p[0], b, 2), 2);
    note("b0", b[0]); note("b1", b[1]);
    CHECK_EQ_I(b[0], 0);     /* observed: the pipe was extended by one zero byte before the data */
    CHECK_EQ_I(b[1], 'x');
    _close(p[0]); _close(p[1]);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(pipe_write_end); return tc_end(); }
