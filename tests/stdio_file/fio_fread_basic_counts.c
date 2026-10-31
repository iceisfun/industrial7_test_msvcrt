/* TC
 * id:    fio_fread_basic_counts
 * area:  stdio_file
 * funcs: fread fopen fclose
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.8.1; MS docs fread
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

static const char *volatile fn = "tc_fio_fread_basic_counts_" TC_CRT_DLL ".tmp";
static const char data[12] = {'a','b','c','d','e','f','g','h','i','j','k','l'};
static size_t volatile v1 = 1, v3 = 3, v4 = 4, v10 = 10;

TC_CASE(bytes10)
{
    char buf[16]; FILE *f; mkfile(fn, data, 12); f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0, sizeof buf);
    CHECK_EQ_U(fread(buf, 1, v10, f), 10); CHECK_MEM(buf, data, 10); fclose(f);
}
TC_CASE(elems4x3)
{
    char buf[16]; FILE *f; f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    memset(buf, 0, sizeof buf);
    CHECK_EQ_U(fread(buf, v4, v3, f), 3); CHECK_MEM(buf, data, 12); fclose(f);
}
TC_CASE(one_big)
{
    char buf[16]; FILE *f; f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(buf, v10, v1, f), 1); CHECK_MEM(buf, data, 10); fclose(f);
}
TC_CASE(successive)
{
    char a[4], b[4]; FILE *f; f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(a, 1, v4, f), 4); CHECK_EQ_U(fread(b, 1, v4, f), 4);
    CHECK_MEM(a, data, 4); CHECK_MEM(b, data + 4, 4); CHECK_EQ_I(ftell(f), 8); fclose(f);
}
int main(void)
{
    tc_begin(); TC_RUN(bytes10); TC_RUN(elems4x3); TC_RUN(one_big); TC_RUN(successive);
    remove(fn); return tc_end();
}
