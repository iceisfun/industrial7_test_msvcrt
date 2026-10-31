/* TC
 * id:    fio_fread_zero_args
 * area:  stdio_file
 * funcs: fread fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fread: size or count 0 returns 0, buffer unchanged
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

static const char *volatile fn = "tc_fio_fread_zero_args_" TC_CRT_DLL ".tmp";
static size_t volatile z = 0, v5 = 5;

TC_CASE(size_zero)
{
    char buf[8]; FILE *f; mkfile(fn, "abcdefgh", 8); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0x5a, sizeof buf);
    CHECK_EQ_U(fread(buf, z, v5, f), 0);
    CHECK_MEM(buf, "ZZZZZZZZ", 8); CHECK_EQ_I(ftell(f), 0); fclose(f);
}
TC_CASE(count_zero)
{
    char buf[8]; FILE *f; f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0x5a, sizeof buf);
    CHECK_EQ_U(fread(buf, v5, z, f), 0);
    CHECK_MEM(buf, "ZZZZZZZZ", 8); CHECK_EQ_I(ftell(f), 0); CHECK(feof(f) == 0); CHECK(ferror(f) == 0); fclose(f);
}
int main(void)
{
    tc_begin(); TC_RUN(size_zero); TC_RUN(count_zero);
    remove(fn); return tc_end();
}
