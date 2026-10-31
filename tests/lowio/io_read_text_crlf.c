/* TC
 * id:    io_read_text_crlf
 * area:  lowio
 * funcs: _read _open _write _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _read: in text mode CR-LF is replaced by LF; _open _O_TEXT/_O_BINARY
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

static char N[] = "tc_io_read_text_crlf.tmp";

static void mk(void)
{
    int fd; _unlink(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) { _write(fd, "a\r\nb\r\n", 6); _close(fd); }
}

TC_CASE(text_mode)
{
    int fd; char b[16] = {0};
    mk();
    fd = _open(N, _O_RDONLY | _O_TEXT);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 4);
    CHECK_MEM(b, "a\nb\n", 4);
    CHECK_EQ_I(_read(fd, b, 16), 0);
    _close(fd);
}
TC_CASE(binary_mode)
{
    int fd; char b[16] = {0};
    fd = _open(N, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 6);
    CHECK_MEM(b, "a\r\nb\r\n", 6);
    _close(fd);
    _unlink(N);
}

int main(void) { tc_begin(); TC_RUN(text_mode); TC_RUN(binary_mode); return tc_end(); }
