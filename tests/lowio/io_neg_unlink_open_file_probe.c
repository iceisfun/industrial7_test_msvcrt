/* TC
 * id:    io_neg_unlink_open_file_probe
 * area:  lowio
 * funcs: _unlink _sopen _close
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sopen (_SH_DENYNO); Win32 DeleteFile requires FILE_SHARE_DELETE on existing opens
 * note:  PROBE. A file opened by _sopen(_SH_DENYNO) is believed NOT to be opened with FILE_SHARE_DELETE, so _unlink
 *        fails with -1/EACCES (sharing violation) and the file survives; after _close it can be unlinked.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_neg_unlink_open_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(open_file_denied)
{
    int fd, r;
    DeleteFileA(fn);
    fd = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    errno = 0;
    r = _unlink(fn);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EACCES);
    CHECK(GetFileAttributesA(fn) != INVALID_FILE_ATTRIBUTES);
    if (fd >= 0) _close(fd);
    CHECK_EQ_I(_unlink(fn), 0);
}
int main(void) { tc_begin(); TC_RUN(open_file_denied); return tc_end(); }
