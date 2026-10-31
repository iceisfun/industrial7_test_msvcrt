/* TC
 * id:    io_open_osfhandle_append_probe
 * area:  lowio
 * funcs: _open_osfhandle _write _close
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _open_osfhandle (flags: _O_APPEND)
 * note:  PROBE. A handle whose file pointer is at 0 wrapped with _O_APPEND is believed to make _write seek to
 *        end-of-file first, so writing "X" to a file holding "abc" yields "abcX". Without it, "Xbc".
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>

static const char *volatile fn = "tc_io_open_osfhandle_app_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(append_flag_seeks_to_end)
{
    HANDLE h; DWORD w = 0, r = 0; int fd; char b[8] = {0};
    h = CreateFileA(fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    WriteFile(h, "abc", 3, &w, NULL); CloseHandle(h);
    h = CreateFileA(fn, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    fd = _open_osfhandle((intptr_t)h, _O_APPEND | _O_BINARY);
    CHECK(fd >= 0);
    CHECK_EQ_I(_write(fd, "X", 1), 1);
    if (fd >= 0) _close(fd);
    h = CreateFileA(fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, b, 7, &r, NULL); CloseHandle(h); }
    CHECK_EQ_I(r, 4);
    CHECK_STR(b, "abcX");
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(append_flag_seeks_to_end); return tc_end(); }
