/* TC
 * id:    io_read_zero_count_probe
 * area:  lowio
 * funcs: _read _open _close _lseek _write _tell
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _read (silent on count 0)
 * note:  PROBE: _read(fd,buf,0) returns 0 and does not move the file position.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_read_zero_count_probe.tmp";

TC_CASE(zero_count)
{
    int fd; char b[4] = {'q', 0, 0, 0};
    _unlink(N);
    fd = _open(N, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    _write(fd, "0123456789", 10);
    _lseek(fd, 2, SEEK_SET);
    CHECK_EQ_I(_read(fd, b, 0), 0);
    CHECK_EQ_I(_tell(fd), 2);
    CHECK_EQ_I(b[0], 'q');
    _close(fd); _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(zero_count); return tc_end(); }
