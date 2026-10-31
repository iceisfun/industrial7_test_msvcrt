/* TC
 * id:    chr_fgets_n2_n3
 * area:  stdio_char
 * funcs: fgets fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2 (reads at most n-1 characters)
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
static const char *volatile path = "tc_chr_fgets_n2_n3_" TC_CRT_DLL ".tmp";

static void fill(char *b, size_t n) { size_t i; for (i = 0; i < n; i++) b[i] = 'S'; }

TC_CASE(n2_one_char)
{
    FILE *f; char b[4]; fill(b, sizeof b); mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 2, f) == b); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'S'); CHECK_EQ_I(b[3], 'S');
    fclose(f);
}
TC_CASE(n2_newline)
{
    FILE *f; char b[4]; fill(b, sizeof b); mk(path, "\nx", 2); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 2, f) == b); CHECK_STR(b, "\n"); CHECK_EQ_I(b[2], 'S');
    CHECK_EQ_I((fgetc)(f), 'x');
    fclose(f);
}
TC_CASE(n3_leaves_rest)
{
    FILE *f; char b[6]; fill(b, sizeof b); mk(path, "abc", 3); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 3, f) == b); CHECK_STR(b, "ab"); CHECK_EQ_I(b[3], 'S'); CHECK_EQ_I(b[5], 'S');
    CHECK_EQ_I((fgetc)(f), 'c');
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(n2_one_char); TC_RUN(n2_newline); TC_RUN(n3_leaves_rest);
    DeleteFileA(path);
    return tc_end();
}
