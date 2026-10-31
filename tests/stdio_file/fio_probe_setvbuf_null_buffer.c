/* TC
 * id:    fio_probe_setvbuf_null_buffer
 * area:  stdio_file
 * funcs: setvbuf fopen fwrite fflush fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.5.6; MS docs setvbuf
 * note:  PROBE: setvbuf(fp, NULL, _IOFBF, 4096) returns 0 and the CRT allocates its own buffer (buffered until fflush). C11 permits a NULL buf (implementation allocates); MS docs do not clearly say.
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

static const char *fn = "tc_fio_probe_setvbuf_null_buffer_a.tmp";

TC_CASE(null_buf)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    r = setvbuf(fp, NULL, _IOFBF, 4096);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 0);
    CHECK_EQ_I(fflush(fp), 0);
    CHECK_EQ_I(fsz(fn), 5);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(null_buf); return tc_end(); }
