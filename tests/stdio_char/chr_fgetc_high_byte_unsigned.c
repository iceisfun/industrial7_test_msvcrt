/* TC
 * id:    chr_fgetc_high_byte_unsigned
 * area:  stdio_char
 * funcs: fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.1 (as unsigned char converted to int); MS docs fgetc
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
static const char *volatile path = "tc_chr_fgetc_high_byte_unsigned_" TC_CRT_DLL ".tmp";

TC_CASE(bytes)
{
    static const unsigned char d[] = { 0x80, 0xFF, 0x00, 0x7F, 0x00 };
    FILE *f; mk(path, d, 5); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 128);
    CHECK_EQ_I((fgetc)(f), 255); CHECK(feof(f) == 0); CHECK_EQ_I(ferror(f), 0);
    CHECK_EQ_I((fgetc)(f), 0); CHECK(feof(f) == 0);
    CHECK_EQ_I((fgetc)(f), 127);
    CHECK_EQ_I((fgetc)(f), 0);
    CHECK_EQ_I((fgetc)(f), EOF);
    fclose(f);
}
TC_CASE(ff_not_eof)
{
    unsigned char d = 0xFF; FILE *f; int c; mk(path, &d, 1); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    c = (fgetc)(f); CHECK(c != EOF); CHECK_EQ_I(c, 255); CHECK(feof(f) == 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(bytes); TC_RUN(ff_not_eof);
    DeleteFileA(path);
    return tc_end();
}
