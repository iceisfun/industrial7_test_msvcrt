/* TC
 * id:    io_dup_lowest_free
 * area:  lowio
 * funcs: _dup _open _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   dup of an fd returns the lowest free descriptor
 * note:  probing: _dup returns the lowest-numbered free descriptor (undocumented, but how the lowio table allocates)
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
#define A "tc_io_dup_lowest_free_a.tmp"
#define B "tc_io_dup_lowest_free_b.tmp"
TC_CASE(lowest)
{
    int a, b, d; rm(A); rm(B);
    a = _open(A, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    b = _open(B, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(a >= 3 && b >= 3);
    if (a < 0 || b < 0) { if (a >= 0) _close(a); if (b >= 0) _close(b); rm(A); rm(B); return; }
    CHECK_EQ_I(_close(a), 0);
    d = _dup(b);
    CHECK_EQ_I(d, a);
    if (d >= 0) _close(d);
    _close(b); rm(A); rm(B);
}
int main(void) { tc_begin(); TC_RUN(lowest); return tc_end(); }
