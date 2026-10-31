/* TC
 * id:    io_neg_creat_readonly_existing
 * area:  lowio
 * funcs: _creat _chmod _unlink _write _close _open _read
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _creat: fails if the existing file is read-only
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_neg_creat_readonly_existing.tmp";

TC_CASE(creat_over_readonly)
{
    int fd; char b[16] = {0};
    _chmod(N, _S_IREAD | _S_IWRITE); _unlink(N);
    fd = _creat(N, _S_IREAD | _S_IWRITE);
    if (fd < 0) { CHECK(fd >= 0); return; }
    CHECK_EQ_I(_write(fd, "keep", 4), 4);
    _close(fd);
    CHECK_EQ_I(_chmod(N, _S_IREAD), 0);
    errno = 0;
    fd = _creat(N, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd, -1);
    CHECK_EQ_I(errno, EACCES);
    if (fd >= 0) _close(fd);
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_read(fd, b, 15), 4); CHECK_STR(b, "keep"); _close(fd); }
    _chmod(N, _S_IREAD | _S_IWRITE); _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(creat_over_readonly); return tc_end(); }
