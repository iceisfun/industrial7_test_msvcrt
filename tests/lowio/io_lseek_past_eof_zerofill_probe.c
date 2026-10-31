/* TC
 * id:    io_lseek_past_eof_zerofill_probe
 * area:  lowio
 * funcs: _lseek _write _filelength _open _close _read
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   Win32 WriteFile/SetFilePointer semantics (NTFS zero-fills the gap)
 * note:  PROBE: bytes between old EOF and a write after seeking past EOF read back as zero.
 *        Believed true on NTFS; docs call extended-region contents unspecified.
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
static const char *fn = "tc_io_lseek_past_eof_zerofill_probe_" TC_S1(TC_CRT_VER) ".tmp";

TC_CASE(gap_zero_filled)
{
    unsigned char buf[128];
    int fd, i, nz = 0;
    fd = _open(fn, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_lseek(fd, 100, SEEK_SET), 100);
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_write(fd, "Z", 1), 1);
    CHECK_EQ_I(_filelength(fd), 101);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, buf, 128), 101);
    for (i = 0; i < 100; i++) if (buf[i] != 0) nz++;
    CHECK_EQ_I(nz, 0);
    CHECK_EQ_I(buf[100], 'Z');
    CHECK_EQ_I(_close(fd), 0);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(gap_zero_filled); return tc_end(); }
