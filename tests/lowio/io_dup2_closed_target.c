/* TC
 * id:    io_dup2_closed_target
 * area:  lowio
 * funcs: _dup2 _open _close _read _write _lseek
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _dup2
 * note:  probing: _dup2 may target a descriptor number that is not currently open and not the lowest free
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
#define N "tc_io_dup2_closed_target_1.tmp"
TC_CASE(free_number)
{
    int a, n, r; char b[4] = {0}; rm(N);
    a = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(a >= 3);
    if (a < 0) return;
    n = a + 2;
    CHECK_EQ_I(_write(a, "qrs", 3), 3);
    r = _dup2(a, n);
    CHECK_EQ_I(r, 0);
    if (r == 0) {
        CHECK_EQ_I(_lseek(n, 0, SEEK_SET), 0);
        CHECK_EQ_I(_read(n, b, 4), 3);
        CHECK_MEM(b, "qrs", 3);
        CHECK_EQ_I(_close(n), 0);
    }
    _close(a); rm(N);
}
int main(void) { tc_begin(); TC_RUN(free_number); return tc_end(); }
