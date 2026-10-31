/* TC
 * id:    fio_setvbuf_modes_visibility
 * area:  stdio_file
 * funcs: setvbuf fopen fwrite fflush fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs setvbuf; C11 7.21.5.6
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

static const char *fn = "tc_fio_setvbuf_modes_visibility_a.tmp";
static char buf1[1024];
static char big[1500];

TC_CASE(full)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    r = setvbuf(fp, buf1, _IOFBF, sizeof buf1);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 0);
    CHECK_EQ_I(fflush(fp), 0);
    CHECK_EQ_I(fsz(fn), 5);
    fclose(fp); DeleteFileA(fn);
}
TC_CASE(nobuf)
{
    FILE *fp = fopen(fn, "wb"); volatile int r;
    CHECK(fp != NULL); if (!fp) return;
    r = setvbuf(fp, NULL, _IONBF, 0);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite("hello", 1, 5, fp), 5);
    CHECK_EQ_I(fsz(fn), 5);
    fclose(fp); DeleteFileA(fn);
}
TC_CASE(overflow)
{
    FILE *fp = fopen(fn, "wb"); volatile int r; long s; int i;
    CHECK(fp != NULL); if (!fp) return;
    for (i = 0; i < (int)sizeof big; i++) big[i] = 'x';
    r = setvbuf(fp, buf1, _IOFBF, sizeof buf1);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(fwrite(big, 1, sizeof big, fp), sizeof big);
    s = fsz(fn);
    CHECK(s >= 0 && s <= (long)sizeof big);
    CHECK_EQ_I(fflush(fp), 0);
    CHECK_EQ_I(fsz(fn), (long)sizeof big);
    fclose(fp); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(full); TC_RUN(nobuf); TC_RUN(overflow); return tc_end(); }
