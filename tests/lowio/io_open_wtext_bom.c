/* TC
 * id:    io_open_wtext_bom
 * area:  lowio
 * funcs: _open _read _close
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _open: _O_WTEXT detects a BOM in the file
 * note:  probing: _open(_O_RDONLY|_O_WTEXT) consumes a UTF-16LE BOM so _read returns only the chars after it; and a file without BOM is read as plain bytes
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
#define N "tc_io_open_wtext_bom_1.tmp"
TC_CASE(with_bom)
{
    static const char d[] = { (char)0xFF, (char)0xFE, 'A', 0, 'B', 0 };
    char b[16] = {0}; int fd, n;
    mk(N, d, 6);
    fd = _open(N, _O_RDONLY | _O_WTEXT);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    n = _read(fd, b, 16);
    CHECK_EQ_I(n, 4);
    CHECK_MEM(b, "A\0B\0", 4);
    _close(fd); rm(N);
}
TC_CASE(no_bom)
{
    char b[16] = {0}; int fd, n;
    mk(N, "AB", 2);
    fd = _open(N, _O_RDONLY | _O_WTEXT);
    CHECK(fd >= 3);
    if (fd < 0) { rm(N); return; }
    n = _read(fd, b, 2);
    CHECK_EQ_I(n, 2);
    CHECK_MEM(b, "AB", 2);
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(with_bom); TC_RUN(no_bom); return tc_end(); }
