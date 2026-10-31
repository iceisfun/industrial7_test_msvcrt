/* TC
 * id:    io_fmode_default
 * area:  lowio
 * funcs: _get_fmode _set_fmode _creat _open _setmode _close
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fmode, _get_fmode, _set_fmode, _creat, _setmode
 * note:  _setmode returns the previous mode, used here to learn the mode of a new fd. The original fmode is restored with _set_fmode(_O_TEXT) (equivalent to the default of 0).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static void mk(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD w = 0;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, (DWORD)cap, &r, NULL); CloseHandle(h); return (int)r;
}
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define N "tc_io_fmode_default_" TC_CRT_DLL ".tmp"

static int mode_of_new_creat(void)
{
    int fd, prev; rm(N);
    fd = _creat(N, _S_IREAD | _S_IWRITE);
    if (fd < 0) return -2;
    prev = _setmode(fd, _O_BINARY);
    _close(fd); rm(N);
    return prev;
}
TC_CASE(initial_text) { int m = -1; CHECK_EQ_I(_get_fmode(&m), 0); CHECK(m == 0 || m == _O_TEXT); }
TC_CASE(creat_text_by_default)
{
    int m = -1;
    _get_fmode(&m);
    if (m != 0 && m != _O_TEXT) TC_SKIP("fmode not default");
    CHECK_EQ_I(mode_of_new_creat(), _O_TEXT);
}
TC_CASE(set_binary)
{
    int m = -1, fd, prev;
    CHECK_EQ_I(_set_fmode(_O_BINARY), 0);
    CHECK_EQ_I(_get_fmode(&m), 0);
    CHECK_EQ_I(m, _O_BINARY);
    CHECK_EQ_I(mode_of_new_creat(), _O_BINARY);
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd >= 0) { prev = _setmode(fd, _O_TEXT); CHECK_EQ_I(prev, _O_BINARY); _close(fd); }
    rm(N);
    _set_fmode(_O_TEXT);
}
TC_CASE(explicit_text_overrides)
{
    int fd, prev;
    CHECK_EQ_I(_set_fmode(_O_BINARY), 0);
    rm(N);
    fd = _open(N, _O_CREAT | _O_WRONLY | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd >= 0) { prev = _setmode(fd, _O_BINARY); CHECK_EQ_I(prev, _O_TEXT); _close(fd); }
    rm(N);
    _set_fmode(_O_TEXT);
}
TC_CASE(restore_text)
{
    int m = -1;
    CHECK_EQ_I(_set_fmode(_O_TEXT), 0);
    CHECK_EQ_I(_get_fmode(&m), 0);
    CHECK_EQ_I(m, _O_TEXT);
    CHECK_EQ_I(mode_of_new_creat(), _O_TEXT);
}

int main(void)
{
    tc_begin();
    TC_RUN(initial_text);
    TC_RUN(creat_text_by_default);
    TC_RUN(set_binary);
    TC_RUN(explicit_text_overrides);
    TC_RUN(restore_text);
    return tc_end();
}
