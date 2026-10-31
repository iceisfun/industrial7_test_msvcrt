/* TC
 * id:    io_read_write_interleave_text
 * area:  lowio
 * funcs: _write _read _lseek _open _close _unlink
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _lseek (text mode seeking), _read
 * note:  PROBE: after writing "a\nb" in text mode (file bytes 61 0D 0A 62), _lseek(fd,2,SEEK_SET) lands between CR and LF;
 *        a text-mode _read then returns the lone LF followed by 'b' ("\nb", 2 bytes). Also probes that seeking to 1 and
 *        reading yields "\nb" (CRLF translated, 2 bytes).
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

static const char *volatile fn = "tc_io_rw_interleave.tmp";

static int mk(void)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_TEXT, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _write(fd, "a\nb", 3);
    return fd;
}
TC_CASE(seek_mid_crlf)
{
    char b[8] = {0}; int fd = mk();
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 2, SEEK_SET), 2);
    CHECK_EQ_I(_read(fd, b, 8), 2);
    CHECK_MEM(b, "\nb", 2);
    _close(fd); _unlink(fn);
}
TC_CASE(seek_before_crlf)
{
    char b[8] = {0}; int fd = mk();
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 1, SEEK_SET), 1);
    CHECK_EQ_I(_read(fd, b, 8), 2);
    CHECK_MEM(b, "\nb", 2);
    _close(fd); _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(seek_mid_crlf); TC_RUN(seek_before_crlf); return tc_end(); }
