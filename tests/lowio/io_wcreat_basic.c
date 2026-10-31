/* TC
 * id:    io_wcreat_basic
 * area:  lowio
 * funcs: _wcreat _close _waccess _wunlink _write _read _wopen
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _creat, _wcreat, _access, _waccess, _unlink
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <wchar.h>

static wchar_t N[] = L"tc_io_wcreat_basic.tmp";

TC_CASE(create_access_unlink)
{
    int fd; _wunlink(N);
    fd = _wcreat(N, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0);
    CHECK_EQ_I(_waccess(N, 0), 0);
    CHECK_EQ_I(_waccess(N, 6), 0);
    CHECK_EQ_I(_wunlink(N), 0);
    errno = 0;
    CHECK_EQ_I(_waccess(N, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(truncates_existing)
{
    int fd; char b[8];
    _wunlink(N);
    fd = _wcreat(N, _S_IREAD | _S_IWRITE);
    if (fd < 0) { CHECK(fd >= 0); return; }
    _write(fd, "abc", 3); _close(fd);
    fd = _wcreat(N, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd);
    fd = _wopen(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_read(fd, b, 8), 0); _close(fd); }
    _wunlink(N);
}

int main(void) { tc_begin(); TC_RUN(create_access_unlink); TC_RUN(truncates_existing); return tc_end(); }
