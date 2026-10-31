/* TC
 * id:    io_u16text_write
 * area:  lowio
 * funcs: _open _write _close _filelength
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _open _O_U16TEXT; _write
 * note:  Probes: _write on a _O_U16TEXT fd returns the byte count, a new _O_U16TEXT file gets a UTF-16LE BOM (FF FE), the payload follows. All assertions here are probes.
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

#define N "tc_io_u16text_write_" TC_CRT_DLL ".tmp"

static int mkfd(void)
{
    rm(N);
    return _open(N, _O_CREAT | _O_WRONLY | _O_U16TEXT, _S_IREAD | _S_IWRITE);
}
TC_CASE(write_returns_count)
{
    int fd = mkfd();
    wchar_t buf[2]; buf[0] = L'a'; buf[1] = L'b';
    CHECK(fd >= 3);
    if (fd >= 0) { CHECK_EQ_I(_write(fd, buf, 4), 4); _close(fd); }
    rm(N);
}
TC_CASE(payload_after_bom)
{
    int fd = mkfd(), n = 0;
    unsigned char d[16];
    wchar_t buf[2]; buf[0] = L'a'; buf[1] = L'b';
    CHECK(fd >= 3);
    if (fd >= 0) { _write(fd, buf, 4); _close(fd); }
    n = slurp(N, (char *)d, sizeof d);
    CHECK(n >= 4);
    if (n >= 4) { CHECK_EQ_I(d[n-4], 'a'); CHECK_EQ_I(d[n-3], 0); CHECK_EQ_I(d[n-2], 'b'); CHECK_EQ_I(d[n-1], 0); }
    rm(N);
}
TC_CASE(bom_written)
{
    int fd = mkfd(), n = 0;
    unsigned char d[16];
    wchar_t buf[2]; buf[0] = L'a'; buf[1] = L'b';
    CHECK(fd >= 3);
    if (fd >= 0) { _write(fd, buf, 4); _close(fd); }
    n = slurp(N, (char *)d, sizeof d);
    CHECK_EQ_I(n, 6);
    if (n >= 2) { CHECK_EQ_I(d[0], 0xFF); CHECK_EQ_I(d[1], 0xFE); }
    rm(N);
}
TC_CASE(filelength)
{
    int fd = mkfd();
    wchar_t buf[2]; buf[0] = L'a'; buf[1] = L'b';
    CHECK(fd >= 3);
    if (fd >= 0) { _write(fd, buf, 4); CHECK_EQ_I(_filelength(fd), 6); _close(fd); }
    rm(N);
}

int main(void)
{
    tc_begin();
    TC_RUN(write_returns_count);
    TC_RUN(payload_after_bom);
    TC_RUN(bom_written);
    TC_RUN(filelength);
    return tc_end();
}
