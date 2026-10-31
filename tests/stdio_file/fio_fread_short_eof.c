/* TC
 * id:    fio_fread_short_eof
 * area:  stdio_file
 * funcs: fread feof fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.8.1, 7.21.10.2 (feof); MS docs fread, feof
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

static const char *volatile fn = "tc_fio_fread_short_eof_" TC_CRT_DLL ".tmp";
static size_t volatile v7 = 7, v20 = 20;

TC_CASE(short_read)
{
    char buf[32]; FILE *f; mkfile(fn, "1234567", 7); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(buf, 1, v20, f), 7); CHECK(feof(f) != 0); CHECK(ferror(f) == 0);
    CHECK_MEM(buf, "1234567", 7);
    CHECK_EQ_U(fread(buf, 1, v20, f), 0); CHECK(feof(f) != 0); CHECK(ferror(f) == 0);
    fclose(f);
}
TC_CASE(exact_no_eof)
{
    char buf[32]; FILE *f; f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(buf, 1, v7, f), 7); CHECK(feof(f) == 0); CHECK(ferror(f) == 0);
    fclose(f);
}
int main(void)
{
    tc_begin(); TC_RUN(short_read); TC_RUN(exact_no_eof);
    remove(fn); return tc_end();
}
