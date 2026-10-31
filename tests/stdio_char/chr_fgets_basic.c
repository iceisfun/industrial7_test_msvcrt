/* TC
 * id:    chr_fgets_basic
 * area:  stdio_char
 * funcs: fgets fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2; MS docs fgets
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
static const char *volatile path = "tc_chr_fgets_basic_" TC_CRT_DLL ".tmp";

TC_CASE(lines)
{
    FILE *f; char b[32]; mk(path, "one\ntwo\n\nlast", 13); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_P(fgets(b, 32, f), b); CHECK_STR(b, "one\n");
    CHECK_EQ_P(fgets(b, 32, f), b); CHECK_STR(b, "two\n");
    CHECK_EQ_P(fgets(b, 32, f), b); CHECK_STR(b, "\n");
    CHECK_EQ_P(fgets(b, 32, f), b); CHECK_STR(b, "last");
    CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(null_at_eof)
{
    FILE *f; char b[8]; mk(path, "q", 1); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 8, f) == b); CHECK_STR(b, "q");
    CHECK(fgets(b, 8, f) == NULL); CHECK(feof(f) != 0); CHECK_EQ_I(ferror(f), 0);
    fclose(f);
}
TC_CASE(empty_file)
{
    FILE *f; char b[8] = "zzz"; mk(path, "", 0); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 8, f) == NULL); CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(long_line_split)
{
    FILE *f; char b[8]; mk(path, "abcdefghij\n", 11); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 6, f) == b); CHECK_STR(b, "abcde");
    CHECK(fgets(b, 8, f) == b); CHECK_STR(b, "fghij\n");
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(lines); TC_RUN(null_at_eof); TC_RUN(empty_file); TC_RUN(long_line_split);
    DeleteFileA(path);
    return tc_end();
}
