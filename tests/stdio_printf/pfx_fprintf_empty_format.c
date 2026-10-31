/* TC
 * id:    pfx_fprintf_empty_format
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ferror
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
#define FN "tc_pfx_fprintf_empty_format_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(empty)
{
    unsigned char b[4]; volatile const char *fmt = "";
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, (const char *)fmt), 0);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 0);
    remove(FN);
}
TC_CASE(empty_string_arg)
{
    unsigned char b[4]; volatile const char *s = "";
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "%s", (const char *)s), 0);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 0);
    remove(FN);
}
TC_CASE(precision_zero_value_zero)
{
    unsigned char b[4]; volatile int z = 0;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "%.0d", z), 0);
    CHECK_EQ_I(ferror(f), 0);
    fclose(f);
    CHECK_EQ_U(slurp(FN, b, sizeof b), 0);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(empty);
    TC_RUN(empty_string_arg);
    TC_RUN(precision_zero_value_zero);
    return tc_end();
}
