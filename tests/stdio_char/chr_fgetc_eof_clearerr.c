/* TC
 * id:    chr_fgetc_eof_clearerr
 * area:  stdio_char
 * funcs: fgetc fopen fclose feof clearerr rewind
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.1, 7.21.10.1 clearerr, 7.21.9.5 rewind; MS docs clearerr/rewind
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
static const char *volatile path = "tc_chr_fgetc_eof_clearerr_" TC_CRT_DLL ".tmp";

TC_CASE(clearerr_clears)
{
    FILE *f; mk(path, "z", 1); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'z'); CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    clearerr(f); CHECK_EQ_I(feof(f), 0);
    CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    fclose(f);
}
TC_CASE(rewind_restarts)
{
    FILE *f; mk(path, "qr", 2); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'q'); CHECK_EQ_I((fgetc)(f), 'r'); CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    rewind(f); CHECK_EQ_I(feof(f), 0);
    CHECK_EQ_I((fgetc)(f), 'q'); CHECK_EQ_I((fgetc)(f), 'r');
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(clearerr_clears); TC_RUN(rewind_restarts);
    DeleteFileA(path);
    return tc_end();
}
