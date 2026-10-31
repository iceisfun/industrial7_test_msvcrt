/* TC
 * id:    fio_fflush_basic_visibility
 * area:  stdio_file
 * funcs: fopen fflush fwrite fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs fflush; C11 7.21.5.2
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

static const char *fn = "tc_fio_fflush_basic_visibility_a.tmp";

TC_CASE(visibility)
{
    FILE *fp = fopen(fn, "wb");
    volatile int r1, r2;
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fwrite("abcd", 1, 4, fp), 4);
    CHECK_EQ_I(fsz(fn), 0);
    r1 = fflush(fp);
    CHECK_EQ_I(r1, 0);
    CHECK_EQ_I(fsz(fn), 4);
    r2 = fflush(fp);
    CHECK_EQ_I(r2, 0);
    CHECK_EQ_I(fsz(fn), 4);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(visibility); return tc_end(); }
