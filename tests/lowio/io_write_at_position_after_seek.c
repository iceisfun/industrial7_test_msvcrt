/* TC
 * id:    io_write_at_position_after_seek
 * area:  lowio
 * funcs: _write _lseek _read _open _close _unlink
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _lseek, _write
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

static const char *volatile fn = "tc_io_write_at_pos.tmp";

TC_CASE(overwrite_middle)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "ABCDEF", 6), 6);
    CHECK_EQ_I(_lseek(fd, 2, SEEK_SET), 2);
    CHECK_EQ_I(_write(fd, "xy", 2), 2);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 4);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 16), 6);
    CHECK_MEM(b, "ABxyEF", 6);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_END), 6);
    _close(fd); _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(overwrite_middle); return tc_end(); }
