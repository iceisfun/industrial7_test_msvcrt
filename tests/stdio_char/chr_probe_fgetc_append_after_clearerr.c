/* TC
 * id:    chr_probe_fgetc_append_after_clearerr
 * area:  stdio_char
 * funcs: fgetc fopen fclose feof clearerr
 * crt:   all
 * tier:  p0
 * conf:  probe
 * note:  Probes: after fgetc hit EOF on a binary read stream, clearerr, then another handle appends a byte, fgetc on the first stream returns the new byte (no sticky CRT-level EOF).
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
static const char *volatile path = "tc_chr_probe_fgetc_append_after_clearerr_" TC_CRT_DLL ".tmp";

TC_CASE(append_after_clearerr)
{
    FILE *f, *g; mk(path, "a", 1); f = fopen(path, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I((fgetc)(f), 'a'); CHECK_EQ_I((fgetc)(f), EOF); CHECK(feof(f) != 0);
    g = fopen(path, "ab"); CHECK(g != NULL);
    if (g) { CHECK_EQ_I((fputc)('b', g), 'b'); CHECK_EQ_I(fclose(g), 0); }
    clearerr(f);
    CHECK_EQ_I((fgetc)(f), 'b'); CHECK_EQ_I((fgetc)(f), EOF);
    fclose(f);
}
int main(void)
{
    tc_begin();
    TC_RUN(append_after_clearerr);
    DeleteFileA(path);
    return tc_end();
}
