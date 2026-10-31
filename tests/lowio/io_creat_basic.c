/* TC
 * id:    io_creat_basic
 * area:  lowio
 * funcs: _creat _close _write _filelength _unlink
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _creat, _wcreat (creates or truncates, opens for writing; translation mode defaults to _fmode, text unless changed)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_creat_basic_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(new_file_write)
{
    int fd;
    _unlink(fn);
    fd = _creat(fn, PM);
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "abcde", 5), 5);
        CHECK_EQ_I(_filelength(fd), 5);
        CHECK_EQ_I(_close(fd), 0);
    }
    CHECK_EQ_I(_access(fn, 0), 0);
    _unlink(fn);
}
TC_CASE(truncates_existing)
{
    int fd;
    _unlink(fn);
    fd = _creat(fn, PM);
    if (fd >= 0) { _write(fd, "0123456789", 10); _close(fd); }
    fd = _creat(fn, PM);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_filelength(fd), 0); _close(fd); }
    _unlink(fn);
}
TC_CASE(default_text_mode_translates_lf)
{
    int fd;
    CHECK_EQ_I(_fmode, 0);   /* default: text; nothing in the test changes it */
    _unlink(fn);
    fd = _creat(fn, PM);
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "a\n", 2), 2);   /* returns bytes of the caller's buffer */
        CHECK_EQ_I(_filelength(fd), 3);        /* LF written as CRLF */
        _close(fd);
    }
    _unlink(fn);
}
TC_CASE(binary_mode_no_translation)
{
    int fd, old = _fmode;
    _unlink(fn);
    _fmode = _O_BINARY;
    fd = _creat(fn, PM);
    _fmode = old;
    CHECK(fd >= 0);
    if (fd >= 0) {
        CHECK_EQ_I(_write(fd, "a\n", 2), 2);
        CHECK_EQ_I(_filelength(fd), 2);
        _close(fd);
    }
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(new_file_write); TC_RUN(truncates_existing);
    TC_RUN(default_text_mode_translates_lf); TC_RUN(binary_mode_no_translation);
    return tc_end();
}
