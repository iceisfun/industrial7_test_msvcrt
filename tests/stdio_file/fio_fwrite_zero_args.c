/* TC
 * id:    fio_fwrite_zero_args
 * area:  stdio_file
 * funcs: fwrite fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.8.2 (size or nmemb zero: returns 0, stream state unchanged); MS docs fwrite
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void mkfile(const char *name, const void *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (n) WriteFile(h, data, n, &w, NULL);
    CloseHandle(h);
}
static DWORD fsize(const char *name)
{
    DWORD s = 0xffffffffu; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = GetFileSize(h, NULL); CloseHandle(h); }
    return s;
}

static const char *volatile fn = "tc_fio_fwrite_zero_args_" TC_CRT_DLL ".tmp";
static size_t volatile z = 0, v5 = 5;

TC_CASE(zero)
{
    static const char d[8] = "abcdefg";
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite(d, z, v5, f), 0);
    CHECK_EQ_U(fwrite(d, v5, z, f), 0);
    CHECK(ferror(f) == 0); CHECK_EQ_I(ftell(f), 0);
    fclose(f); CHECK_EQ_U(fsize(fn), 0);
}
int main(void) { tc_begin(); TC_RUN(zero); remove(fn); return tc_end(); }
