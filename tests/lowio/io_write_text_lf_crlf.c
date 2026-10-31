/* TC
 * id:    io_write_text_lf_crlf
 * area:  lowio
 * funcs: _write _open _close _filelength _read
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _write (text mode LF -> CRLF; does not affect return value)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_write_text_lf_crlf_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(text_expands)
{
    char buf[16];
    int fd, n;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    n = _write(fd, "a\nb\n", 4);
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(_filelength(fd), 6);
    CHECK_EQ_I(_close(fd), 0);
    fd = _open(fn, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) { _unlink(fn); return; }
    CHECK_EQ_I(_read(fd, buf, 16), 6);
    CHECK_MEM(buf, "a\r\nb\r\n", 6);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
TC_CASE(binary_no_expand)
{
    int fd;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\nb\n", 4), 4);
    CHECK_EQ_I(_filelength(fd), 4);
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(text_expands); TC_RUN(binary_no_expand); return tc_end(); }
