/* TC
 * id:    gio_setmode_midstream_translate
 * area:  lowio
 * funcs: _setmode _open _write _read _lseek _tell _close _unlink
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmode (sets translation mode; returns previous mode)
 * note:  Translation applies to I/O performed after the switch; the file position is untouched.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_setmode_midstream_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(write_binary_read_text)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\r\nb", 4), 4);
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_BINARY);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 4), 3);
    CHECK_MEM(b, "a\nb", 3);
    _close(fd); _unlink(fn);
}
TC_CASE(write_text_then_binary)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_TEXT, PM);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "x\n", 2), 2);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 8), 3);
    CHECK_MEM(b, "x\r\n", 3);
    _close(fd); _unlink(fn);
}
TC_CASE(returns_previous_both_ways)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_BINARY);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_BINARY);
    _close(fd); _unlink(fn);
}
TC_CASE(position_unchanged)
{
    int fd; long before; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3);
    if (fd < 0) return;
    _write(fd, "abcdef", 6);
    before = _tell(fd);
    CHECK_EQ_I(before, 6);
    _setmode(fd, _O_TEXT);
    CHECK_EQ_I(_tell(fd), before);
    _setmode(fd, _O_BINARY);
    CHECK_EQ_I(_tell(fd), before);
    _close(fd); _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(write_binary_read_text); TC_RUN(write_text_then_binary);
    TC_RUN(returns_previous_both_ways); TC_RUN(position_unchanged);
    return tc_end();
}
