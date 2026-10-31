/* TC
 * id:    chr_putc_alias
 * area:  stdio_char
 * funcs: putc fputc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.8 putc; MS docs fputc/putc
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
static const char *volatile path = "tc_chr_putc_alias_" TC_CRT_DLL ".tmp";

TC_CASE(return_and_order)
{
    FILE *f = fopen(path, "wb"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((putc)('1', f), '1'); CHECK_EQ_I((fputc)('2', f), '2'); CHECK_EQ_I((putc)('3', f), '3'); CHECK_EQ_I((fputc)('4', f), '4');
    fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 4); CHECK_MEM(b, "1234", 4);
}
TC_CASE(text_crlf)
{
    FILE *f = fopen(path, "wt"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((putc)('\n', f), 10); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 2); CHECK_EQ_I(b[0], 13); CHECK_EQ_I(b[1], 10);
}
int main(void)
{
    tc_begin();
    TC_RUN(return_and_order); TC_RUN(text_crlf);
    DeleteFileA(path);
    return tc_end();
}
