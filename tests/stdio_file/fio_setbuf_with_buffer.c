/* TC
 * id:    fio_setbuf_with_buffer
 * area:  stdio_file
 * funcs: setbuf fopen fwrite fflush fclose
 * crt:   all
 * tier:  p1
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

static const char *fn = "tc_fio_setbuf_with_buffer_a.tmp";
static char sbuf[BUFSIZ];

TC_CASE(buffered)
{
    FILE *fp = fopen(fn, "wb");
    CHECK(fp != NULL); if (!fp) return;
    setbuf(fp, sbuf);
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 0);
    CHECK_EQ_I(fflush(fp), 0);
    CHECK_EQ_I(fsz(fn), 5);
    CHECK_EQ_U(fwrite("world", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 5);
    CHECK_EQ_I(fclose(fp), 0);
    CHECK_EQ_I(fsz(fn), 10);
    DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(buffered); return tc_end(); }
