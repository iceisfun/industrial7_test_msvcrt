/* TC
 * id:    chr_fputc_text_crlf
 * area:  stdio_char
 * funcs: fputc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: LF translated to CR-LF on output)
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
static const char *volatile path = "tc_chr_fputc_text_crlf_" TC_CRT_DLL ".tmp";

TC_CASE(text_lf_to_crlf)
{
    FILE *f = fopen(path, "wt"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fputc)('\n', f), 10); CHECK_EQ_I((fputc)('x', f), 'x'); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 3); CHECK_EQ_I(b[0], 0x0D); CHECK_EQ_I(b[1], 0x0A); CHECK_EQ_I(b[2], 'x');
}
TC_CASE(text_cr_alone)
{
    FILE *f = fopen(path, "wt"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fputc)('\r', f), 13); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 0x0D);
}
TC_CASE(binary_lf_only)
{
    FILE *f = fopen(path, "wb"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fputc)('\n', f), 10); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 1); CHECK_EQ_I(b[0], 0x0A);
}
int main(void)
{
    tc_begin();
    TC_RUN(text_lf_to_crlf); TC_RUN(text_cr_alone); TC_RUN(binary_lf_only);
    DeleteFileA(path);
    return tc_end();
}
