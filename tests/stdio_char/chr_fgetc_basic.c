/* TC
 * id:    chr_fgetc_basic
 * area:  stdio_char
 * funcs: fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.1; MS docs fgetc
 * note:  binary mode; (fgetc)(f) defeats header macros
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
static const char *volatile path = "tc_chr_fgetc_basic_" TC_CRT_DLL ".tmp";

TC_CASE(sequence)
{
    FILE *f; mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'a'); CHECK_EQ_I((fgetc)(f), 'b'); CHECK_EQ_I((fgetc)(f), 'c');
    CHECK_EQ_I(feof(f), 0);
    CHECK_EQ_I((fgetc)(f), EOF);
    CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    CHECK_EQ_I((fgetc)(f), EOF); CHECK_EQ_I((fgetc)(f), EOF);
    CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
TC_CASE(empty_file)
{
    FILE *f; mk(path, "", 0); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(sequence); TC_RUN(empty_file);
    DeleteFileA(path);
    return tc_end();
}
