/* TC
 * id:    io_creat_readonly_pmode
 * area:  lowio
 * funcs: _creat _close _access _chmod _unlink _write
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _creat, _open (pmode), _access
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_creat_readonly_pmode.tmp";

static void cleanup(void) { _chmod(N, _S_IREAD | _S_IWRITE); _unlink(N); }

TC_CASE(handle_writable)
{
    int fd; cleanup();
    fd = _creat(N, _S_IREAD);
    CHECK(fd >= 0);
    if (fd >= 0) {
        static char d[] = "x";
        CHECK_EQ_I(_write(fd, d, 1), 1);
        CHECK_EQ_I(_close(fd), 0);
    }
    cleanup();
}
TC_CASE(access_write_denied_after_close)
{
    int fd; cleanup();
    fd = _creat(N, _S_IREAD);
    if (fd < 0) { CHECK(fd >= 0); return; }
    _close(fd);
    errno = 0;
    CHECK_EQ_I(_access(N, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK_EQ_I(_access(N, 0), 0);
    CHECK_EQ_I(_access(N, 4), 0);
    CHECK_EQ_I(_chmod(N, _S_IWRITE), 0);
    CHECK_EQ_I(_access(N, 2), 0);
    cleanup();
}

int main(void) { tc_begin(); TC_RUN(handle_writable); TC_RUN(access_write_denied_after_close); return tc_end(); }
