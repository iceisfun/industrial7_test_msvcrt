/* TC
 * id:    fio_neg_fopen_s_bad_args
 * area:  stdio_file
 * kind:  negative
 * funcs: 
 * procs: fopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s: returns errno_t; *pFile set to NULL on failure; invalid parameter handler for NULL args
 * note:  Return of ENOENT for a missing file is the errno value fopen_s documents ('error code'); other cases are Parameter validation behaviour.
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
#define F "tc_fio_neg_fopen_s_bad_args_" TC_S1(TC_CRT_VER) ".tmp"

TC_DECL_PROC(int, fopen_s, (FILE **, const char *, const char *));

TC_CASE(null_pfile)
{
    int r; tc_iph_count = 0;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(NULL, F, "w"); CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1); CHECK(!exists(F));
}
TC_CASE(null_filename)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; volatile const char *nm = NULL; tc_iph_count = 0;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, (const char *)nm, "r"); CHECK_EQ_I(r, EINVAL); CHECK(f == NULL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_mode)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; volatile const char *m = NULL; tc_iph_count = 0;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, F, (const char *)m); CHECK_EQ_I(r, EINVAL); CHECK(f == NULL); CHECK(tc_iph_count >= 1);
}
TC_CASE(invalid_mode)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; tc_iph_count = 0;
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, F, "q"); CHECK_EQ_I(r, EINVAL); CHECK(f == NULL); CHECK(tc_iph_count >= 1); CHECK(!exists(F));
}
TC_CASE(missing_file_returns_enoent)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; del(F);
    if (!tc_p_fopen_s) TC_SKIP("fopen_s not exported");
    r = tc_p_fopen_s(&f, F, "r"); CHECK_EQ_I(r, ENOENT); CHECK(f == NULL);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(fopen_s); CHECK(tc_iph_install());
    TC_RUN(null_pfile); TC_RUN(null_filename); TC_RUN(null_mode); TC_RUN(invalid_mode); TC_RUN(missing_file_returns_enoent);
    return tc_end();
}
