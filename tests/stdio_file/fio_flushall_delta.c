/* TC
 * id:    fio_flushall_delta
 * area:  stdio_file
 * funcs: _flushall fopen fclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _flushall (returns the number of open streams)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static long fsz(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s; if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h); return (long)s;
}

static const char *fa = "tc_fio_flushall_delta_a.tmp", *fb = "tc_fio_flushall_delta_b.tmp";

TC_CASE(delta)
{
    int n0 = _flushall(); FILE *a, *b;
    a = fopen(fa, "w"); b = fopen(fb, "w");
    CHECK(a && b); if (!a || !b) return;
    CHECK_EQ_I(_flushall(), n0 + 2);
    fclose(a); fclose(b);
    CHECK_EQ_I(_flushall(), n0);
    DeleteFileA(fa); DeleteFileA(fb);
}
TC_CASE(writes_pending)
{
    FILE *a = fopen(fa, "wb");
    CHECK(a != NULL); if (!a) return;
    CHECK_EQ_U(fwrite("xyz", 1, 3, a), 3);
    CHECK_EQ_I(fsz(fa), 0);
    _flushall();
    CHECK_EQ_I(fsz(fa), 3);
    fclose(a); DeleteFileA(fa);
}
int main(void) { tc_begin(); TC_RUN(delta); TC_RUN(writes_pending); return tc_end(); }
