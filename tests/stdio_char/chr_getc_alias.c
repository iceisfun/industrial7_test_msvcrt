/* TC
 * id:    chr_getc_alias
 * area:  stdio_char
 * funcs: getc fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.5 getc; MS docs fgetc/getc
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
static const char *volatile path = "tc_chr_getc_alias_" TC_CRT_DLL ".tmp";

TC_CASE(interleave)
{
    FILE *f; mk(path, "wxyz", 4); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((getc)(f), 'w'); CHECK_EQ_I((fgetc)(f), 'x'); CHECK_EQ_I((getc)(f), 'y'); CHECK_EQ_I((fgetc)(f), 'z');
    CHECK_EQ_I((getc)(f), EOF); CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(high_byte)
{
    unsigned char d = 0xFF; FILE *f; mk(path, &d, 1); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((getc)(f), 255); CHECK_EQ_I((getc)(f), EOF);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(interleave); TC_RUN(high_byte);
    DeleteFileA(path);
    return tc_end();
}
