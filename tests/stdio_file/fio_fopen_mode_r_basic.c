/* TC
 * id:    fio_fopen_mode_r_basic
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fread
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3; MS docs fopen
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)   /* file size via kernel32, -1 if missing */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static void del(const char *n) { DeleteFileA(n); }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_fio_fopen_mode_r_basic_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(read_five)
{
    char b[16]; FILE *f; put(F, "abcde", 5);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 5, f), 5); CHECK_MEM(b, "abcde", 5); fclose(f); }
    del(F);
}
TC_CASE(eof)
{
    char b[16]; FILE *f; put(F, "abcde", 5);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 5, f), 5); CHECK_EQ_U(fread(b, 1, 1, f), 0); CHECK(feof(f) != 0); fclose(f); }
    del(F);
}
TC_CASE(empty_file)
{
    char b[4]; FILE *f; put(F, "", 0);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 4, f), 0); CHECK(feof(f) != 0); CHECK_EQ_I(fclose(f), 0); }
    del(F);
}
int main(void) { tc_begin(); TC_RUN(read_five); TC_RUN(eof); TC_RUN(empty_file); return tc_end(); }
