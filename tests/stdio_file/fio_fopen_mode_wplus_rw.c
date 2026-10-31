/* TC
 * id:    fio_fopen_mode_wplus_rw
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fread fwrite rewind
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
#define F "tc_fio_fopen_mode_wplus_rw_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(truncates_existing)
{
    char b[16]; FILE *f; put(F, "0123456789", 10);
    f = fopen(F, "w+"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 4, f), 0); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 0); del(F);
}
TC_CASE(write_rewind_read)
{
    char b[16]; FILE *f = fopen(F, "w+"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fwrite("abc", 1, 3, f), 3);
        rewind(f);
        CHECK_EQ_U(fread(b, 1, 16, f), 3); CHECK_MEM(b, "abc", 3);
        fclose(f);
    }
    del(F);
}
TC_CASE(binary_forms_same)
{
    static const char *const modes[2] = { "w+b", "wb+" };
    int k;
    for (k = 0; k < 2; k++) {
        char b[16]; FILE *f = fopen(F, modes[k]); CHECK(f != NULL);
        if (f) {
            CHECK_EQ_U(fwrite("x\ny", 1, 3, f), 3);
            rewind(f);
            CHECK_EQ_U(fread(b, 1, 16, f), 3); CHECK_MEM(b, "x\ny", 3);
            CHECK_EQ_I(fclose(f), 0);
        }
        CHECK_EQ_I(slurp(F, b, 16), 3);
    }
    del(F);
}
int main(void) { tc_begin(); TC_RUN(truncates_existing); TC_RUN(write_rewind_read); TC_RUN(binary_forms_same); return tc_end(); }
