/* TC
 * id:    io_u8text_write
 * area:  lowio
 * funcs: _open _write _close
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _open _O_U8TEXT; _write
 * note:  Probes: wide characters written to a _O_U8TEXT fd are stored as UTF-8 (U+00E9 -> C3 A9, LF -> CRLF), and the return value is the input byte count. Content is checked at the file tail only, since whether a UTF-8 BOM is written is not asserted.
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

#define N "tc_io_u8text_write_" TC_CRT_DLL ".tmp"

static int mkfd(void)
{
    rm(N);
    return _open(N, _O_CREAT | _O_WRONLY | _O_U8TEXT, _S_IREAD | _S_IWRITE);
}
TC_CASE(utf8_bytes)
{
    int fd = mkfd(), n = 0; unsigned char d[16]; wchar_t c = 0x00E9;
    CHECK(fd >= 3);
    if (fd >= 0) { _write(fd, &c, (unsigned)sizeof c); _close(fd); }
    n = slurp(N, (char *)d, sizeof d);
    CHECK(n >= 2);
    if (n >= 2) { CHECK_EQ_I(d[n-2], 0xC3); CHECK_EQ_I(d[n-1], 0xA9); }
    rm(N);
}
TC_CASE(return_input_count)
{
    int fd = mkfd(); wchar_t c = 0x00E9;
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_write(fd, &c, (unsigned)sizeof c), 2); _close(fd); }
    rm(N);
}
TC_CASE(lf_to_crlf)
{
    int fd = mkfd(), n = 0; unsigned char d[16]; wchar_t c = L'\n';
    CHECK(fd >= 3);
    if (fd >= 0) { _write(fd, &c, (unsigned)sizeof c); _close(fd); }
    n = slurp(N, (char *)d, sizeof d);
    CHECK(n >= 2);
    if (n >= 2) { CHECK_EQ_I(d[n-2], 0x0D); CHECK_EQ_I(d[n-1], 0x0A); }
    rm(N);
}

int main(void)
{
    tc_begin();
    TC_RUN(utf8_bytes);
    TC_RUN(return_input_count);
    TC_RUN(lf_to_crlf);
    return tc_end();
}
