/* TC
 * id:    io_open_osfhandle_flags
 * area:  lowio
 * funcs: _open_osfhandle _setmode _close
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _open_osfhandle (flags _O_TEXT, _O_RDONLY, _O_APPEND); _setmode returns the previous translation mode
 * note:  _O_APPEND behaviour is probed in io_open_osfhandle_append_probe.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>

static const char *volatile fn = "tc_io_open_osfhandle_flags_" TC_S(TC_CRT_VER) ".tmp";
static HANDLE opn(void)
{
    return CreateFileA(fn, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

TC_CASE(text_reported)
{
    int fd, old;
    fd = _open_osfhandle((intptr_t)opn(), _O_TEXT);
    CHECK(fd >= 0);
    old = _setmode(fd, _O_BINARY);
    CHECK_EQ_I(old, _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_BINARY);
    if (fd >= 0) _close(fd);
    DeleteFileA(fn);
}
TC_CASE(binary_reported)
{
    int fd, old;
    fd = _open_osfhandle((intptr_t)opn(), _O_BINARY);
    CHECK(fd >= 0);
    old = _setmode(fd, _O_TEXT);
    CHECK_EQ_I(old, _O_BINARY);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    if (fd >= 0) _close(fd);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(text_reported); TC_RUN(binary_reported); return tc_end(); }
