/* TC
 * id:    io_open_temporary_deletes
 * area:  lowio
 * funcs: _open _close _access _unlink _write
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _open (_O_TEMPORARY: file is deleted when the last file descriptor is closed)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_open_temporary_" TCV ".tmp";

TC_CASE(deleted_on_close)
{
    int fd;
    _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY | _O_TEMPORARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "abc", 3), 3);
    CHECK_EQ_I(_access(fn, 0), 0);
    CHECK_EQ_I(_close(fd), 0);
    errno = 0;
    CHECK_EQ_I(_access(fn, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
    _unlink(fn);
}
TC_CASE(without_flag_persists)
{
    int fd;
    _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) CHECK_EQ_I(_close(fd), 0);
    CHECK_EQ_I(_access(fn, 0), 0);
    CHECK_EQ_I(_unlink(fn), 0);
}
int main(void) { tc_begin(); TC_RUN(deleted_on_close); TC_RUN(without_flag_persists); return tc_end(); }
