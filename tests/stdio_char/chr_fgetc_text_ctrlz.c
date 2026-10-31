/* TC
 * id:    chr_fgetc_text_ctrlz
 * area:  stdio_char
 * funcs: fgetc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (Ctrl+Z is treated as end-of-file in text mode reads)
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
static const char *volatile path = "tc_chr_fgetc_text_ctrlz_" TC_CRT_DLL ".tmp";

TC_CASE(text_stops)
{
    FILE *f; mk(path, "ab\x1a" "cd", 5); f = fopen(path, "rt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'a'); CHECK_EQ_I((fgetc)(f), 'b');
    CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(binary_continues)
{
    FILE *f; mk(path, "ab\x1a" "cd", 5); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'a'); CHECK_EQ_I((fgetc)(f), 'b'); CHECK_EQ_I((fgetc)(f), 0x1a);
    CHECK_EQ_I((fgetc)(f), 'c'); CHECK_EQ_I((fgetc)(f), 'd'); CHECK_EQ_I((fgetc)(f), EOF);
    fclose(f);
}
TC_CASE(ends_with_ctrlz)
{
    FILE *f; mk(path, "x\x1a", 2); f = fopen(path, "rt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'x'); CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
TC_CASE(first_byte_ctrlz)
{
    FILE *f; mk(path, "\x1a" "zz", 3); f = fopen(path, "rt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(text_stops); TC_RUN(binary_continues); TC_RUN(ends_with_ctrlz); TC_RUN(first_byte_ctrlz);
    DeleteFileA(path);
    return tc_end();
}
