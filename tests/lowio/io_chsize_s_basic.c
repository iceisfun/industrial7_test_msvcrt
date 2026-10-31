/* TC
 * id:    io_chsize_s_basic
 * area:  lowio
 * funcs: _chsize_s _open _filelengthi64 _close _write
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _chsize_s: returns 0 on success
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
#define N "tc_io_chsize_s_basic_1.tmp"
TC_CASE(sizes)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_chsize_s(fd, 100), 0);
    CHECK_EQ_I(_filelengthi64(fd), 100);
    CHECK_EQ_I(_chsize_s(fd, 100), 0);
    CHECK_EQ_I(_filelengthi64(fd), 100);
    CHECK_EQ_I(_chsize_s(fd, 40), 0);
    CHECK_EQ_I(_filelengthi64(fd), 40);
    CHECK_EQ_I(_chsize_s(fd, 0), 0);
    CHECK_EQ_I(_filelengthi64(fd), 0);
    _close(fd); rm(N);
}
TC_CASE(written_then_extend)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "xyz", 3), 3);
    CHECK_EQ_I(_chsize_s(fd, 7), 0);
    CHECK_EQ_I(_filelengthi64(fd), 7);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(sizes); TC_RUN(written_then_extend); return tc_end(); }
