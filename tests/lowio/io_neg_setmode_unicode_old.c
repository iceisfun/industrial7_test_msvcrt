/* TC
 * id:    io_neg_setmode_unicode_old
 * area:  lowio
 * funcs: _setmode _open _close
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmode (unicode modes introduced in VS2005)
 * note:  probing: msvcrt.dll (pre-VS2005 CRT) rejects _O_WTEXT/_O_U8TEXT/_O_U16TEXT with -1 EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
#ifndef _O_WTEXT
#define _O_WTEXT 0x10000
#endif
#ifndef _O_U16TEXT
#define _O_U16TEXT 0x20000
#endif
#ifndef _O_U8TEXT
#define _O_U8TEXT 0x40000
#endif
#define N "tc_io_neg_setmode_unicode_old_1.tmp"
TC_CASE(wtext_rejected)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    errno = 0; CHECK_EQ_I(_setmode(fd, _O_WTEXT), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; CHECK_EQ_I(_setmode(fd, _O_U8TEXT), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; CHECK_EQ_I(_setmode(fd, _O_U16TEXT), -1); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_BINARY);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(wtext_rejected); return tc_end(); }
