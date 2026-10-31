/* TC
 * id:    io_setmode_u16_prev
 * area:  lowio
 * funcs: _setmode _open _close
 * crt:   >=90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _setmode
 * note:  probing: the value returned as 'previous mode' after _O_U16TEXT then _O_WTEXT is set (expected _O_WTEXT: the CRT keeps one utf16 state; _O_U16TEXT is the alternative)
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
#define N "tc_io_setmode_u16_prev_1.tmp"
TC_CASE(prev_after_u16)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 3);
    if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_U16TEXT), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_WTEXT);  /* U16TEXT and WTEXT are both stored as 'utf16le'; WTEXT is reported */
    _close(fd); rm(N);
}
int main(void) { tc_begin(); TC_RUN(prev_after_u16); return tc_end(); }
