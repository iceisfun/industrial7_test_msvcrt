/* TC
 * id:    fio_fflush_null_all
 * area:  stdio_file
 * funcs: fflush fopen fwrite fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * note:  relies on default full buffering of disk files (size 0 before flush) - true of MS CRTs, not mandated by C
 * ref:   C11 7.21.5.2; MS docs fflush
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

static const char *fa = "tc_fio_fflush_null_all_a.tmp", *fb = "tc_fio_fflush_null_all_b.tmp";

TC_CASE(flush_all)
{
    FILE *a = fopen(fa, "wb"), *b = fopen(fb, "wb");
    volatile int rc;
    CHECK(a && b); if (!a || !b) return;
    fwrite("12345", 1, 5, a); fwrite("12", 1, 2, b);
    CHECK_EQ_I(fsz(fa), 0); CHECK_EQ_I(fsz(fb), 0);
    rc = fflush(NULL);
    CHECK_EQ_I(rc, 0);
    CHECK_EQ_I(fsz(fa), 5); CHECK_EQ_I(fsz(fb), 2);
    fclose(a); fclose(b); DeleteFileA(fa); DeleteFileA(fb);
}
int main(void) { tc_begin(); TC_RUN(flush_all); return tc_end(); }
