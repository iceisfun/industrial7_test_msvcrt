/* TC
 * id:    io_read_basic
 * area:  lowio
 * funcs: _read _open _close _lseek _write _tell
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _read: returns bytes read, 0 at EOF
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_read_basic.tmp";
static char data[] = "0123456789";

static int mk(void)
{
    int fd; _unlink(N);
    fd = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) { _write(fd, data, 10); _lseek(fd, 0, SEEK_SET); }
    return fd;
}

TC_CASE(partial_then_tail_then_eof)
{
    int fd = mk(); char b[32] = {0};
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 4), 4);
    CHECK_MEM(b, "0123", 4);
    CHECK_EQ_I(_read(fd, b, 100), 6);
    CHECK_MEM(b, "456789", 6);
    CHECK_EQ_I(_tell(fd), 10);
    CHECK_EQ_I(_read(fd, b, 10), 0);
    CHECK_EQ_I(_read(fd, b, 10), 0);
    _close(fd); _unlink(N);
}
TC_CASE(read_after_seek)
{
    int fd = mk(); char b[8] = {0};
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 7, SEEK_SET), 7);
    CHECK_EQ_I(_read(fd, b, 8), 3);
    CHECK_MEM(b, "789", 3);
    _close(fd); _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(partial_then_tail_then_eof); TC_RUN(read_after_seek); return tc_end(); }
