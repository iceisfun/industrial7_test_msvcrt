/* TC
 * id:    fio_setvbuf_iolbf_is_full
 * area:  stdio_file
 * funcs: setvbuf fopen fwrite fclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs setvbuf (_IOLBF treated as _IOFBF on Win32)
 * note:  MS docs: _IOLBF is not supported on Win32 and is treated as _IOFBF (no flush at newline).
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

static const char *fn = "tc_fio_setvbuf_iolbf_is_full_a.tmp";
static char buf1[512];

TC_CASE(lbf)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    r = setvbuf(fp, buf1, _IOLBF, sizeof buf1);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite("abc\n", 1, 4, fp), 4);
    CHECK_EQ_I(fsz(fn), 0);
    CHECK_EQ_I(fflush(fp), 0);
    CHECK_EQ_I(fsz(fn), 4);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(lbf); return tc_end(); }
