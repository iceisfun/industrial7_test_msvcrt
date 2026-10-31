/* TC
 * id:    chr_fputc_basic
 * area:  stdio_char
 * funcs: fputc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.3; MS docs fputc
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
static const char *volatile path = "tc_chr_fputc_basic_" TC_CRT_DLL ".tmp";

TC_CASE(write_and_verify)
{
    FILE *f = fopen(path, "wb"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fputc)('A', f), 'A'); CHECK_EQ_I((fputc)('B', f), 'B'); CHECK_EQ_I((fputc)('C', f), 'C');
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 3); CHECK_MEM(b, "ABC", 3);
}
TC_CASE(nul_char)
{
    FILE *f = fopen(path, "wb"); unsigned char b[8]; DWORD n; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fputc)('\0', f), 0); CHECK_EQ_I((fputc)('x', f), 'x');
    CHECK_EQ_I(ferror(f), 0); fclose(f);
    n = slurp(path, b, 8); CHECK_EQ_U(n, 2); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'x');
}
TC_CASE(roundtrip_fread)
{
    FILE *f = fopen(path, "wb"); char b[8] = {0}; size_t n; CHECK(f != NULL); if (!f) return;
    (fputc)('h', f); (fputc)('i', f); fclose(f);
    f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    n = fread(b, 1, 8, f); CHECK_EQ_U(n, 2); CHECK_STR(b, "hi"); fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(write_and_verify); TC_RUN(nul_char); TC_RUN(roundtrip_fread);
    DeleteFileA(path);
    return tc_end();
}
