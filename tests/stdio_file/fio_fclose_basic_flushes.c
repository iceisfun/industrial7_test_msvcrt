/* TC
 * id:    fio_fclose_basic_flushes
 * area:  stdio_file
 * funcs: fopen fclose fwrite
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * note:  relies on default full buffering of disk files (size 0 before flush) - true of MS CRTs, not mandated by C
 * ref:   C11 7.21.5.1; MS docs fclose
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

static const char *fn = "tc_fio_fclose_basic_flushes_a.tmp";

TC_CASE(flush_and_remove)
{
    FILE *fp = fopen(fn, "wb");
    volatile int rc;
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("abcdef", 1, 6, fp), 6);
    CHECK_EQ_I(fsz(fn), 0);  /* still buffered */
    rc = fclose(fp);
    CHECK_EQ_I(rc, 0);
    CHECK_EQ_I(fsz(fn), 6);
}
TC_CASE(read_stream)
{
    FILE *fp = fopen(fn, "rb");
    volatile int rc;
    CHECK(fp != NULL); if (!fp) return;
    rc = fclose(fp);
    CHECK_EQ_I(rc, 0);
}
TC_CASE(removable)
{
    CHECK(DeleteFileA(fn));  /* fails with sharing violation if still open */
    CHECK_EQ_I(fsz(fn), -1);
}
int main(void) { tc_begin(); TC_RUN(flush_and_remove); TC_RUN(read_stream); TC_RUN(removable); return tc_end(); }
