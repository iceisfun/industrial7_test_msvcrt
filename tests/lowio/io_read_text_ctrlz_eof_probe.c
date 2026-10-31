/* TC
 * id:    io_read_text_ctrlz_eof_probe
 * area:  lowio
 * funcs: _read _open _write _close _eof
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _eof
 * note:  PROBE: after a text-mode read stops at Ctrl+Z in a disk file, _eof(fd) reports 1 (file pointer not moved back).
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_read_text_ctrlz_eof_probe.tmp";

TC_CASE(eof_after_ctrlz)
{
    int fd; char b[16];
    _unlink(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    _write(fd, "ab\x1a" " cd", 6); _close(fd);
    fd = _open(N, _O_RDONLY | _O_TEXT);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 2);
    CHECK_EQ_I(_read(fd, b, 16), 0);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd); _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(eof_after_ctrlz); return tc_end(); }
