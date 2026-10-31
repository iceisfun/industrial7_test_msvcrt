/* TC
 * id:    fio_setbuf_null_unbuffered
 * area:  stdio_file
 * funcs: setbuf fopen fwrite fputc fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.5.5; MS docs setbuf
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

static long fsz(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    DWORD s; if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL); CloseHandle(h); return (long)s;
}

static const char *fn = "tc_fio_setbuf_null_unbuffered_a.tmp";

TC_CASE(write_visible)
{
    FILE *fp = fopen(fn, "wb");
    CHECK(fp != NULL); if (!fp) return;
    setbuf(fp, NULL);
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 5);
    fclose(fp); DeleteFileA(fn);
}
TC_CASE(putc_visible)
{
    FILE *fp = fopen(fn, "wb"); int i;
    CHECK(fp != NULL); if (!fp) return;
    setbuf(fp, NULL);
    for (i = 1; i <= 3; i++) {
        CHECK_EQ_I(fputc('a' + i, fp), 'a' + i);
        CHECK_EQ_I(fsz(fn), i);
    }
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(write_visible); TC_RUN(putc_visible); return tc_end(); }
