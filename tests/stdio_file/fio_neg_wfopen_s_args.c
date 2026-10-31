/* TC
 * id:    fio_neg_wfopen_s_args
 * area:  stdio_file
 * kind:  negative
 * funcs:
 * procs: _wfopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen_s, _wfopen_s (returns zero on success, error code on failure)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *wfopen_s_t)(FILE **, const wchar_t *, const wchar_t *);
static wfopen_s_t p;
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
#define NAME "tc_fio_neg_wfopen_s_ok_" SFX ".tmp"
static void mk(const char *n, const char *d)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0, l = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    while (d[l]) l++;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, char *b, int cap)  /* returns bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}

TC_CASE(null_pfile)
{
    int r; tc_iph_count = 0; if (!p) TC_SKIP("not exported");
    r = p(NULL, L"tc_fio_neg_wfopen_s.tmp", L"r");
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_name)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; const wchar_t *volatile name = NULL; tc_iph_count = 0; if (!p) TC_SKIP("not exported");
    r = p(&f, name, L"r");
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);  /* *pFile state: see fio_neg_probe_wfopen_s_null_pfile_state */
}
TC_CASE(missing_file)
{
    FILE *f = (FILE *)(ULONG_PTR)1; int r; if (!p) TC_SKIP("not exported");
    r = p(&f, L"tc_fio_neg_wfopen_s_missing.tmp", L"r");
    CHECK_EQ_I(r, ENOENT); CHECK(f == NULL);
}
TC_CASE(success)
{
    FILE *f = NULL; int r; wchar_t wn[64]; if (!p) TC_SKIP("not exported");
    MultiByteToWideChar(CP_ACP, 0, NAME, -1, wn, 64);
    mk(NAME, "abc");
    r = p(&f, wn, L"rb");
    CHECK_EQ_I(r, 0); CHECK(f != NULL);
    if (f) fclose(f);
    DeleteFileA(NAME);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    p = (wfopen_s_t)tc_crt_proc("_wfopen_s");
    TC_RUN(null_pfile); TC_RUN(null_name); TC_RUN(missing_file); TC_RUN(success);
    return tc_end();
}
