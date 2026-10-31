/* TC
 * id:    gio_chsize_position_probe
 * area:  lowio
 * funcs: _chsize _tell _open _write _close _unlink _filelength
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _chsize (silent about file pointer)
 * note:  PROBES that _chsize leaves the file position unchanged when extending (pos stays 3), and records what happens
 *        when shrinking below the current position (only that the call succeeds and the file length is the new size).
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_chsize_position_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(extend_pos_unchanged)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3); if (fd < 0) return;
    _write(fd, "abc", 3);
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_tell(fd), 3);
    _close(fd); _unlink(fn);
}
TC_CASE(shrink_before_pos)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3); if (fd < 0) return;
    _write(fd, "abcdefgh", 8);
    CHECK_EQ_I(_chsize(fd, 2), 0);
    CHECK_EQ_I(_filelength(fd), 2);
    CHECK_EQ_I(_tell(fd), 8);   /* probe: position is restored, not clamped */
    _close(fd); _unlink(fn);
}
TC_CASE(write_after_shrink_extends)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, PM);
    CHECK(fd >= 3); if (fd < 0) return;
    _write(fd, "abcdefgh", 8);
    CHECK_EQ_I(_chsize(fd, 2), 0);
    CHECK_EQ_I(_write(fd, "Z", 1), 1);
    CHECK_EQ_I(_filelength(fd), 9);   /* probe: written at old position 8 */
    _close(fd); _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(extend_pos_unchanged); TC_RUN(shrink_before_pos); TC_RUN(write_after_shrink_extends);
    return tc_end();
}
