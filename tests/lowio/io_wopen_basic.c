/* TC
 * id:    io_wopen_basic
 * area:  lowio
 * funcs: _wopen _close _waccess _wunlink _write
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _open, _wopen; _access, _waccess; _unlink, _wunlink
 * note:  The path contains U+00E9 and U+4E2D (BMP, non-ASCII), written with \u escapes so source encoding does not matter.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <wchar.h>
#include <sys/stat.h>

static const wchar_t *volatile wfn = L"tc_io_wopen_basic_\u00e9\u4e2d_" TCV ".tmp";

TC_CASE(create_write_close_unlink)
{
    int fd;
    DWORD attr;
    _wunlink(wfn);
    fd = _wopen(wfn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "hello", 5), 5);
        CHECK_EQ_I(_close(fd), 0);
    }
    CHECK_EQ_I(_waccess(wfn, 0), 0);
    CHECK_EQ_I(_waccess(wfn, 2), 0);
    attr = GetFileAttributesW(wfn);   /* the name really is the wide name */
    CHECK(attr != INVALID_FILE_ATTRIBUTES);
    CHECK_EQ_I(_wunlink(wfn), 0);
    errno = 0;
    CHECK_EQ_I(_waccess(wfn, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(reopen_readonly)
{
    int fd;
    char b[8] = {0};
    _wunlink(wfn);
    fd = _wopen(wfn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) { _write(fd, "xyz", 3); _close(fd); }
    fd = _wopen(wfn, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_read(fd, b, 8), 3); CHECK_MEM(b, "xyz", 3); _close(fd); }
    _wunlink(wfn);
}
int main(void) { tc_begin(); TC_RUN(create_write_close_unlink); TC_RUN(reopen_readonly); return tc_end(); }
