/* TC
 * id:    io_read_text_ctrlz
 * area:  lowio
 * funcs: _read _open _write _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _read: in text mode Ctrl+Z is treated as end of file; _open _O_TEXT
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_read_text_ctrlz.tmp";

static void mk(void)
{
    int fd; _unlink(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) { _write(fd, "ab\x1a" " cd", 6); _close(fd); }
}

TC_CASE(text_mode)
{
    int fd; char b[16] = {0};
    mk();
    fd = _open(N, _O_RDONLY | _O_TEXT);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 2);
    CHECK_MEM(b, "ab", 2);
    CHECK_EQ_I(_read(fd, b, 16), 0);
    _close(fd);
}
TC_CASE(binary_mode)
{
    int fd; char b[16] = {0};
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 6);
    CHECK_MEM(b, "ab\x1a" " cd", 6);
    _close(fd);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(text_mode); TC_RUN(binary_mode); return tc_end(); }
