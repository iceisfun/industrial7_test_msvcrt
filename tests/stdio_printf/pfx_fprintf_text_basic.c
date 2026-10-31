/* TC
 * id:    pfx_fprintf_text_basic
 * area:  stdio_printf
 * funcs: fprintf fopen fclose fread
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1, 7.21.3; MS docs fprintf, fopen
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
#define FN "tc_pfx_fprintf_text_basic_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(roundtrip)
{
    char buf[64] = {0}; size_t n; int r;
    volatile int v = 42; volatile const char *s = "abc"; volatile int c = 'z'; volatile unsigned x = 255;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%d %s %c %x", v, (const char *)s, c, x);
    CHECK_EQ_I(r, 11);
    CHECK_EQ_I(fclose(f), 0);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    n = fread(buf, 1, sizeof buf - 1, f);
    CHECK_EQ_U(n, 11);
    CHECK_STR(buf, "42 abc z ff");
    CHECK_EQ_I(fclose(f), 0);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(roundtrip);
    return tc_end();
}
