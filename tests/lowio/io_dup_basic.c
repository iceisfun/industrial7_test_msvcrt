/* TC
 * id:    io_dup_basic
 * area:  lowio
 * funcs: _dup _open _close _write _tell _lseek
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _dup: second descriptor associated with the same file; file pointer shared
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#define N "tc_io_dup_basic_1.tmp"
TC_CASE(shares_position)
{
    int fd, d; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    d = _dup(fd);
    CHECK(d >= 3);
    CHECK(d != fd);
    if (d < 0) { _close(fd); rm(N); return; }
    CHECK_EQ_I(_write(d, "abc", 3), 3);
    CHECK_EQ_I(_tell(fd), 3);
    CHECK_EQ_I(_write(fd, "de", 2), 2);
    CHECK_EQ_I(_tell(d), 5);
    CHECK_EQ_I(_lseek(d, 1, SEEK_SET), 1);
    CHECK_EQ_I(_tell(fd), 1);
    _close(d); _close(fd); rm(N);
}
TC_CASE(close_original)
{
    int fd, d; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    d = _dup(fd);
    CHECK(d >= 3);
    if (d < 0) { _close(fd); rm(N); return; }
    CHECK_EQ_I(_close(fd), 0);
    CHECK_EQ_I(_write(d, "xy", 2), 2);
    CHECK_EQ_I(_tell(d), 2);
    CHECK_EQ_I(_close(d), 0);
    rm(N);
}
int main(void) { tc_begin(); TC_RUN(shares_position); TC_RUN(close_original); return tc_end(); }
