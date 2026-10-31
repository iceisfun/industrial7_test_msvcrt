/* TC
 * id:    pfx_fprintf_percent_c_nul
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1 (%c converts int to unsigned char and writes it); MS docs printf %c
 * note:  Windows CRT writes the NUL produced by %c with value 0 to the stream and counts it; this is well known behaviour but not spelled out in the docs.
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
#define FN "tc_pfx_fprintf_percent_c_nul_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(plain)
{
    unsigned char b[16]; DWORD n; volatile int z = 0;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a%cb", z), 3);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 3);
    CHECK_MEM(b, "a\0b", 3);
    remove(FN);
}
TC_CASE(width)
{
    unsigned char b[16]; DWORD n; volatile int z = 0;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "%5c", z), 5);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 5);
    CHECK_MEM(b, "    \0", 5);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(plain);
    TC_RUN(width);
    return tc_end();
}
