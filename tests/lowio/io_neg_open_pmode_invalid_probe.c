/* TC
 * id:    io_neg_open_pmode_invalid_probe
 * area:  lowio
 * funcs: _open _close _write
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _open: pmode _S_IREAD/_S_IWRITE (0x100/0x80 style bits); other bits are not documented
 * note:  Probes _open(O_CREAT|O_WRONLY) with undefined pmode bits (0x7777) and pmode 0. Assumed: the call still succeeds (only _S_IWRITE matters), file is created, the returned fd is writable even though the file may be created read-only (no _S_IWRITE bit). rc/errno and iph count are recorded as TC INFO.
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
#define N "tc_io_neg_open_pmode_invalid_probe_1.tmp"
static void one(int pm)
{
    int fd, c = tc_iph_count; volatile int pmode = pm;
    rm(N); errno = 0;
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_BINARY, pmode);
    note("pmode", pm); note("fd", fd); note("errno", errno); note("iph", tc_iph_count - c);
    CHECK(fd >= 3);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "hi", 2), 2);
        _close(fd);
        CHECK(GetFileAttributesA(N) != INVALID_FILE_ATTRIBUTES);
        CHECK((GetFileAttributesA(N) & FILE_ATTRIBUTE_READONLY) != 0);   /* no _S_IWRITE bit => read-only file */
    }
    rm(N);
}
TC_CASE(pmode_7777) { one(0x7777); }
TC_CASE(pmode_zero) { one(0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(pmode_7777); TC_RUN(pmode_zero); rm(N); return tc_end(); }
