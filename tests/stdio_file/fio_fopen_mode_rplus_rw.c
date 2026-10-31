/* TC
 * id:    fio_fopen_mode_rplus_rw
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fread fwrite fseek
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3; MS docs fopen
 * note:  Spec case 'r+ on missing file fails' is in fio_neg_fopen_missing_file.
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
#define F "tc_fio_fopen_mode_rplus_rw_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(reads_existing)
{
    char b[16]; FILE *f; put(F, "hello world", 11);
    f = fopen(F, "r+"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 11, f), 11); CHECK_MEM(b, "hello world", 11); fclose(f); }
    del(F);
}
TC_CASE(overwrite_no_truncate)
{
    char b[16]; FILE *f; put(F, "hello world", 11);
    f = fopen(F, "r+"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("HELLO", 1, 5, f), 5); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 11); CHECK_MEM(b, "HELLO world", 11); del(F);
}
TC_CASE(seek_between_read_write)
{
    char b[16]; FILE *f; put(F, "hello world", 11);
    f = fopen(F, "r+"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fread(b, 1, 5, f), 5); CHECK_MEM(b, "hello", 5);
        CHECK_EQ_I(fseek(f, 0, SEEK_CUR), 0);
        CHECK_EQ_U(fwrite("_", 1, 1, f), 1);
        CHECK_EQ_I(fseek(f, 0, SEEK_CUR), 0);
        CHECK_EQ_U(fread(b, 1, 5, f), 5); CHECK_MEM(b, "world", 5);
        CHECK_EQ_I(fclose(f), 0);
    }
    CHECK_EQ_I(slurp(F, b, 16), 11); CHECK_MEM(b, "hello_world", 11); del(F);
}
int main(void) { tc_begin(); TC_RUN(reads_existing); TC_RUN(overwrite_no_truncate); TC_RUN(seek_between_read_write); return tc_end(); }
