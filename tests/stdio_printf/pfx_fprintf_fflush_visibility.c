/* TC
 * id:    pfx_fprintf_fflush_visibility
 * area:  stdio_printf
 * funcs: fprintf fopen fflush setvbuf fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fflush, setvbuf
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static DWORD slurp(const char *n, unsigned char *b, DWORD cap)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return (DWORD)-1;
    if (!ReadFile(h, b, cap, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return got;
}

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_fflush_visibility_" TCS(TC_CRT_VER) ".tmp"
static DWORD fsize(const char *n)
{
    DWORD s; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return (DWORD)-1;
    s = GetFileSize(h, NULL); CloseHandle(h); return s;
}
static char vb[4096];
TC_CASE(visible_after_flush)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, vb, _IOFBF, sizeof vb), 0);
    CHECK_EQ_I(fprintf(f, "hello %d", 12), 8);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_U(fsize(FN), 8);
    CHECK_EQ_I(fprintf(f, "!!"), 2);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_U(fsize(FN), 10);
    fclose(f); remove(FN);
}
TC_CASE(flush_nothing_pending)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fflush(f), 0);
    fprintf(f, "z");
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_U(fsize(FN), 1);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(visible_after_flush);
    TC_RUN(flush_nothing_pending);
    return tc_end();
}
