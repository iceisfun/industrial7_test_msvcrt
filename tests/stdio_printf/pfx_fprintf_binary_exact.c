/* TC
 * id:    pfx_fprintf_binary_exact
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: 'b' = binary mode, no translation
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
#define FN "tc_pfx_fprintf_binary_exact_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(lf_only)
{
    unsigned char b[16]; DWORD n; int r;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "a\nb\n");
    CHECK_EQ_I(r, 4);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 4);
    CHECK_MEM(b, "a\nb\n", 4);
    remove(FN);
}
TC_CASE(crlf)
{
    unsigned char b[16]; DWORD n; int r1, r2;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    r1 = fprintf(f, "\r\n");
    r2 = fprintf(f, "%s", "\n");
    CHECK_EQ_I(r1, 2); CHECK_EQ_I(r2, 1);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, (DWORD)(r1 + r2));
    CHECK_MEM(b, "\r\n\n", 3);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(lf_only);
    TC_RUN(crlf);
    return tc_end();
}
