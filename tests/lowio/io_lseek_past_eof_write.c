/* TC
 * id:    io_lseek_past_eof_write
 * area:  lowio
 * funcs: _lseek _write _filelength _open _close _read
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _lseek (pointer may be positioned beyond EOF), _filelength
 * note:  zero-fill of the gap is NOT asserted here (see io_lseek_past_eof_zerofill_probe)
 */
#include "tc.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_lseek_past_eof_write_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(gap_zero_filled)
{
    unsigned char buf[128];
    int fd;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 100, SEEK_SET), 100);
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_write(fd, "Z", 1), 1);
    CHECK_EQ_I(_filelength(fd), 101);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, buf, 128), 101);
    CHECK_EQ_I(buf[100], 'Z');
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(gap_zero_filled); return tc_end(); }
