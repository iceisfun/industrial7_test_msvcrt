/* TC
 * id:    fio_fopen_mode_a_appends
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fwrite fseek
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3 (writes always at end); MS docs fopen
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
#define F "tc_fio_fopen_mode_a_appends_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(creates)
{
    FILE *f; del(F);
    f = fopen(F, "a"); CHECK(f != NULL);
    if (f) CHECK_EQ_I(fclose(f), 0);
    CHECK(exists(F)); del(F);
}
TC_CASE(keeps_old_data)
{
    char b[16]; FILE *f; put(F, "AB", 2);
    f = fopen(F, "a"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("CD", 1, 2, f), 2); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 4); CHECK_MEM(b, "ABCD", 4); del(F);
}
TC_CASE(seek_zero_still_appends)
{
    char b[16]; FILE *f; put(F, "AB", 2);
    f = fopen(F, "a"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0); CHECK_EQ_U(fwrite("EF", 1, 2, f), 2); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 4); CHECK_MEM(b, "ABEF", 4); del(F);
}
int main(void) { tc_begin(); TC_RUN(creates); TC_RUN(keeps_old_data); TC_RUN(seek_zero_still_appends); return tc_end(); }
