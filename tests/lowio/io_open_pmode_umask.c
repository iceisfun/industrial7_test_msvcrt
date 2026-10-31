/* TC
 * id:    io_open_pmode_umask
 * area:  lowio
 * funcs: _open _umask _access _chmod _unlink _close
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _umask, _open (pmode is applied only when _O_CREAT creates the file; permission setting is pmode & ~mask)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_open_pmode_umask_" TCV ".tmp";

static void cleanup(void) { _chmod(fn, _S_IREAD | _S_IWRITE); _unlink(fn); }

TC_CASE(umask_write_bit_masks_pmode)
{
    int old, fd;
    cleanup();
    old = _umask(_S_IWRITE);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    _umask(old);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd);
    CHECK_EQ_I(_access(fn, 0), 0);
    CHECK_EQ_I(_access(fn, 4), 0);
    errno = 0;
    CHECK_EQ_I(_access(fn, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    cleanup();
}
TC_CASE(umask_zero_keeps_write)
{
    int old, fd;
    cleanup();
    old = _umask(0);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    _umask(old);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd);
    CHECK_EQ_I(_access(fn, 2), 0);
    cleanup();
}
TC_CASE(pmode_read_only_alone)
{
    int old, fd;
    cleanup();
    old = _umask(0);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD);
    _umask(old);
    CHECK(fd >= 0);   /* the descriptor returned at creation is still writable */
    if (fd >= 0) _close(fd);
    errno = 0;
    CHECK_EQ_I(_access(fn, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    cleanup();
}
TC_CASE(umask_returns_previous)
{
    int old = _umask(_S_IREAD), prev;
    prev = _umask(old);
    CHECK_EQ_I(prev, _S_IREAD);
    CHECK_EQ_I(_umask(old), old);
}
int main(void)
{
    tc_begin();
    TC_RUN(umask_write_bit_masks_pmode); TC_RUN(umask_zero_keeps_write);
    TC_RUN(pmode_read_only_alone); TC_RUN(umask_returns_previous);
    return tc_end();
}
