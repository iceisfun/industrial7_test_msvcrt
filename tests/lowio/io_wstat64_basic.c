/* TC
 * id:    io_wstat64_basic
 * area:  lowio
 * funcs: _wstat64 _wopen _close _write _wunlink
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _stat, _wstat (st_dev/st_rdev: drive number of the disk containing the file)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const wchar_t fn[] = L"tc_io_wstat64_basic_\x00e9\x4e2d.tmp";

TC_CASE(wide_name)
{
    struct _stat64 st; int fd; wchar_t cwd[MAX_PATH]; DWORD n;
    DeleteFileW(fn);
    fd = _wopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "1234567", 7), 7);
    _close(fd);
    CHECK(GetFileAttributesW(fn) != INVALID_FILE_ATTRIBUTES);
    memset(&st, 0, sizeof st);
    CHECK_EQ_I(_wstat64(fn, &st), 0);
    CHECK_EQ_I(st.st_size, 7);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFREG);
    CHECK_EQ_I(st.st_dev, st.st_rdev);
    n = GetCurrentDirectoryW(MAX_PATH, cwd);
    if (n > 2 && cwd[1] == L':') CHECK(st.st_dev < 26);
    CHECK_EQ_I(_wunlink(fn), 0);
    DeleteFileW(fn);
}
int main(void) { tc_begin(); TC_RUN(wide_name); return tc_end(); }
