/* TC
 * id:    pfx_fprintf_ftell_text_crlf
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell fflush
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs ftell / fopen text mode
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
#define FN "tc_pfx_fprintf_ftell_text_crlf_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(after_flush)
{
    unsigned char b[16]; int r; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "a\nb\n");
    CHECK_EQ_I(r, 4);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(ftell(f), 6);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 6);
    fclose(f); remove(FN);
}
TC_CASE(after_close_size)
{
    unsigned char b[16]; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    fprintf(f, "x\ny\n");
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(ftell(f), 6);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 6);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(after_flush);
    TC_RUN(after_close_size);
    return tc_end();
}
