/* TC
 * id:    io_filelengthi64_big
 * area:  lowio
 * funcs: _filelengthi64 _open _close _get_osfhandle
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _filelengthi64 (64-bit length)
 * note:  The file is extended past 4 GB with kernel32 (sparse flag + SetEndOfFile) because _chsize_s zero-fills in user mode
 *        (too slow / needs real disk space). Skips when the filesystem cannot do it. Probe-free: only _filelengthi64 is asserted.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <winioctl.h>
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)

static const char *volatile fname = "tc_io_filelengthi64_big_" TC_STR(TC_CRT_VER) ".tmp";
TC_CASE(over_4g)
{
    LARGE_INTEGER p; DWORD ret; HANDLE h; int fd;
    fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    h = (HANDLE)_get_osfhandle(fd);
    if (!DeviceIoControl(h, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &ret, NULL)) { _close(fd); TC_SKIP("no sparse file support"); }
    p.QuadPart = 0x10000000ALL;
    if (!SetFilePointerEx(h, p, NULL, FILE_BEGIN) || !SetEndOfFile(h)) { _close(fd); TC_SKIP("cannot extend file"); }
    CHECK_EQ_I(_filelengthi64(fd), 0x10000000ALL);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(over_4g);
    _unlink(fname);
    return tc_end();
}
