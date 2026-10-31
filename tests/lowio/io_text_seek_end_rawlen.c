/* TC
 * id:    io_text_seek_end_rawlen
 * area:  lowio
 * funcs: _write _lseek _filelength _open _close _unlink
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _write (text mode LF -> CR-LF), _lseek (offsets are raw bytes), _filelength
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

static const char *volatile fn = "tc_io_text_seek_end.tmp";

TC_CASE(raw_length)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\nb", 3), 3);      /* returns bytes of caller's buffer */
    CHECK_EQ_I(_lseek(fd, 0, SEEK_END), 4);    /* a CR LF b */
    CHECK_EQ_I(_filelength(fd), 4);
    _close(fd); _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(raw_length); return tc_end(); }
