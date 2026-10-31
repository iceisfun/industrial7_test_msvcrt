/* TC
 * id:    io_setmode_basic
 * area:  lowio
 * funcs: _setmode _open _close
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmode: returns the previous translation mode
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
#define N "tc_io_setmode_basic_1.tmp"
TC_CASE(toggle)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_BINARY);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_BINARY);
    _close(fd); rm(N);
}
TC_CASE(text_open)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(toggle); TC_RUN(text_open); return tc_end(); }
