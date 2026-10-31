/* TC
 * id:    io_commit_basic
 * area:  lowio
 * funcs: _commit _open _write _close _filelength
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _commit (returns 0 on success)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, int ro)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
    if (ro) SetFileAttributesA(n, FILE_ATTRIBUTE_READONLY);
}

static const char *volatile fn = "tc_io_commit_basic.tmp";
TC_CASE(commit_written_file)
{
    int fd, n;
    rm(fn);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    n = _write(fd, "0123456789", 10);
    CHECK_EQ_I(n, 10);
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I(_close(fd), 0);
    rm(fn);
}
TC_CASE(commit_twice_and_length)
{
    int fd;
    rm(fn);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abcde", 5), 5);
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I(_write(fd, "fgh", 3), 3);
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I((int)_filelength(fd), 8);   /* not the directory entry: it may be stale while open */
    _close(fd);
    rm(fn);
}
int main(void) { tc_begin(); TC_RUN(commit_written_file); TC_RUN(commit_twice_and_length); return tc_end(); }
