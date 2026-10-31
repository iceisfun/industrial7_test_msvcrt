/* TC
 * id:    fio_fopen_s_basic
 * area:  stdio_file
 * kind:  positive
 * funcs: fclose fwrite fread rewind
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s, _wfopen_s
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
#define F "tc_fio_fopen_s_basic_" TC_S1(TC_CRT_VER) ".tmp"

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));

TC_CASE(returns_zero_sets_file)
{
    FILE *f = NULL; int r;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, F, "w"); CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) fclose(f);
    del(F);
}
TC_CASE(write_close_read)
{
    char b[8]; FILE *f = NULL;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    CHECK_EQ_I(tc_p_fopen_s(&f, F, "w"), 0);
    if (f) { CHECK_EQ_U(fwrite("abc", 1, 3, f), 3); CHECK_EQ_I(fclose(f), 0); }
    f = NULL;
    CHECK_EQ_I(tc_p_fopen_s(&f, F, "r"), 0);
    if (f) { CHECK_EQ_U(fread(b, 1, 8, f), 3); CHECK_MEM(b, "abc", 3); fclose(f); }
    del(F);
}
TC_CASE(wplusb_rewind_read)
{
    char b[8]; FILE *f = NULL;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    CHECK_EQ_I(tc_p_fopen_s(&f, F, "w+b"), 0);
    if (f) {
        CHECK_EQ_U(fwrite("x\ny", 1, 3, f), 3); rewind(f);
        CHECK_EQ_U(fread(b, 1, 8, f), 3); CHECK_MEM(b, "x\ny", 3); fclose(f);
    }
    del(F);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fopen_s);
    TC_RUN(returns_zero_sets_file); TC_RUN(write_close_read); TC_RUN(wplusb_rewind_read);
    return tc_end();
}
