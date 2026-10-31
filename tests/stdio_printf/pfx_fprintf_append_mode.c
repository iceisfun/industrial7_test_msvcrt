/* TC
 * id:    pfx_fprintf_append_mode
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: "a" opens for writing at end of file, creating it if missing
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
#define FN "tc_pfx_fprintf_append_mode_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(text_append)
{
    unsigned char b[16]; DWORD n; FILE *f;
    remove(FN);
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "one"), 3); fclose(f);
    f = fopen(FN, "a"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(fprintf(f, "two"), 3); fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 6);
    CHECK_MEM(b, "onetwo", 6);
    remove(FN);
}
TC_CASE(binary_append)
{
    unsigned char b[16]; DWORD n; FILE *f;
    remove(FN);
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fprintf(f, "a\n"); fclose(f);
    f = fopen(FN, "ab"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(fprintf(f, "b\n"), 2); fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 4);
    CHECK_MEM(b, "a\nb\n", 4);
    remove(FN);
}
TC_CASE(append_creates)
{
    unsigned char b[16]; DWORD n; FILE *f;
    remove(FN);
    f = fopen(FN, "a"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "new"), 3); fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 3);
    CHECK_MEM(b, "new", 3);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(text_append);
    TC_RUN(binary_append);
    TC_RUN(append_creates);
    return tc_end();
}
