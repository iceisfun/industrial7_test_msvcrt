/* TC
 * id:    chr_fputc_conversion
 * area:  stdio_char
 * funcs: fputc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.3 (c converted to unsigned char; returns the character written)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *p, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, n, &w, NULL); CloseHandle(h); }
}
static DWORD slurp(const char *p, unsigned char *b, DWORD cap)
{
    DWORD r = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (!ReadFile(h, b, cap, &r, NULL)) r = 0; CloseHandle(h); }
    return r;
}
static const char *volatile path = "tc_chr_fputc_conversion_" TC_CRT_DLL ".tmp";

static volatile int v;
TC_CASE(truncates_to_uchar)
{
    FILE *f = fopen(path, "wb"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    v = 0x141; CHECK_EQ_I((fputc)(v, f), 0x41);
    v = -1;    CHECK_EQ_I((fputc)(v, f), 255);
    v = EOF;   CHECK_EQ_I((fputc)(v, f), 255);
    v = 256;   CHECK_EQ_I((fputc)(v, f), 0);
    v = 0x1FF; CHECK_EQ_I((fputc)(v, f), 255);
    CHECK_EQ_I(ferror(f), 0); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 5);
    CHECK_EQ_I(b[0], 0x41); CHECK_EQ_I(b[1], 0xFF); CHECK_EQ_I(b[2], 0xFF); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 0xFF);
}
int main(void)
{
    tc_begin();
    TC_RUN(truncates_to_uchar);
    DeleteFileA(path);
    return tc_end();
}
