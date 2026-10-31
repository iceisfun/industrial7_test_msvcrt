/* TC
 * id:    fio_neg_fopen_missing_file
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.5.3 (r requires existing file); MS docs fopen
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
#define F "tc_fio_neg_fopen_missing_file_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(r_missing)
{
    FILE *f; del(F); errno = 0;
    f = fopen(F, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
}
TC_CASE(rplus_missing)
{
    FILE *f; del(F); errno = 0;
    f = fopen(F, "r+"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
    CHECK(!exists(F));
    if (f) fclose(f);
}
TC_CASE(rb_missing_dir_component)
{
    FILE *f; errno = 0;
    f = fopen("tc_fio_no_such_dir_xyz\\sub\\file.tmp", "rb"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
}
TC_CASE(w_missing_dir_component)
{
    FILE *f; errno = 0;
    f = fopen("tc_fio_no_such_dir_xyz\\sub\\file.tmp", "w"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
}
int main(void) { tc_begin(); TC_RUN(r_missing); TC_RUN(rplus_missing); TC_RUN(rb_missing_dir_component); TC_RUN(w_missing_dir_component); return tc_end(); }
