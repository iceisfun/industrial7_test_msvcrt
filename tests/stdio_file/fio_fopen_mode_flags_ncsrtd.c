/* TC
 * id:    fio_fopen_mode_flags_ncsrtd
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fclose fwrite fread remove
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (c, n, N, S, R, T, D mode flags)
 * note:  msvcr80/msvcr90/msvcrt covered by probe test fio_probe_fopen_flags_legacy
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
#define F "tc_fio_fopen_mode_flags_ncsrtd_" TC_S1(TC_CRT_VER) ".tmp"

static void try_write(const char *mode)
{
    char b[4]; FILE *f = fopen(F, mode); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("z", 1, 1, f), 1); CHECK_EQ_I(fclose(f), 0); CHECK_EQ_I(slurp(F, b, 4), 1); CHECK_MEM(b, "z", 1); }
    remove(F);
}
TC_CASE(w_N) { try_write("wN"); }
TC_CASE(w_c) { try_write("wc"); }
TC_CASE(w_S) { try_write("wS"); }
TC_CASE(w_R) { try_write("wR"); }
TC_CASE(w_T) { try_write("wT"); }
TC_CASE(w_D_deletes_on_close)
{
    FILE *f = fopen(F, "wD"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("z", 1, 1, f), 1); CHECK_EQ_I(fclose(f), 0); }
    CHECK(!exists(F));
    errno = 0; f = fopen(F, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
    remove(F);
}
TC_CASE(read_hints)
{
    static const char *const modes[2] = { "rS", "rR" };
    int k;
    for (k = 0; k < 2; k++) {
        char b[16]; FILE *f; put(F, "hello", 5);
        f = fopen(F, modes[k]); CHECK(f != NULL);
        if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 5); CHECK_MEM(b, "hello", 5); fclose(f); }
    }
    remove(F);
}
TC_CASE(combined_wbSN) { try_write("wbSN"); }
int main(void)
{
    tc_begin();
    TC_RUN(w_N); TC_RUN(w_c); TC_RUN(w_S); TC_RUN(w_R); TC_RUN(w_T); TC_RUN(w_D_deletes_on_close);
    TC_RUN(read_hints); TC_RUN(combined_wbSN);
    return tc_end();
}
