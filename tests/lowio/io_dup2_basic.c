/* TC
 * id:    io_dup2_basic
 * area:  lowio
 * funcs: _dup2 _open _close _write _read _lseek
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _dup2: returns 0 on success; the second descriptor's previous file is closed
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
#define A "tc_io_dup2_basic_a.tmp"
#define B "tc_io_dup2_basic_b.tmp"
static int op(const char *n) { rm(n); return _open(n, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE); }
TC_CASE(redirect)
{
    int a = op(A), b = op(B), r; char buf[8] = {0};
    CHECK(a >= 3 && b >= 3 && a != b);
    if (a < 0 || b < 0) { if (a >= 0) _close(a); if (b >= 0) _close(b); rm(A); rm(B); return; }
    CHECK_EQ_I(_write(a, "hello", 5), 5);
    r = _dup2(a, b);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(_lseek(b, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(b, buf, 8), 5);
    CHECK_MEM(buf, "hello", 5);
    _close(a); _close(b); rm(A); rm(B);
}
TC_CASE(same_fd)
{
    int a = op(A);
    CHECK(a >= 3);
    if (a < 0) return;
    CHECK_EQ_I(_dup2(a, a), 0);
    CHECK_EQ_I(_write(a, "z", 1), 1);
    _close(a); rm(A);
}
TC_CASE(close_source)
{
    int a = op(A), b = op(B);
    CHECK(a >= 3 && b >= 3);
    if (a < 0 || b < 0) { if (a >= 0) _close(a); if (b >= 0) _close(b); rm(A); rm(B); return; }
    CHECK_EQ_I(_dup2(a, b), 0);
    CHECK_EQ_I(_close(a), 0);
    CHECK_EQ_I(_write(b, "qq", 2), 2);
    CHECK_EQ_I(_close(b), 0);
    rm(A); rm(B);
}
int main(void) { tc_begin(); TC_RUN(redirect); TC_RUN(same_fd); TC_RUN(close_source); return tc_end(); }
