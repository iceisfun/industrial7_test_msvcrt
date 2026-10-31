/* TC
 * id:    fio_neg_freopen_s_args
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen _fcloseall
 * procs: freopen_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs freopen_s (returns zero on success, error code on failure)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *freopen_s_t)(FILE **, const char *, const char *, FILE *);
static freopen_s_t p;
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)

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

#define F "tc_fio_neg_freopen_s_" SFX ".tmp"

TC_CASE(null_pfile)
{
    FILE *fp; int r; if (!p) TC_SKIP("not exported"); mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0;
    r = p(NULL, F, "r", fp);
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();  /* whether the failed call closed fp is unspecified */
    DeleteFileA(F);
}
TC_CASE(null_stream)
{
    FILE *out = NULL; int r; FILE *volatile s = NULL; if (!p) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = p(&out, F, "r", s);
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_mode)
{
    FILE *fp, *out = NULL; int r; const char *volatile m = NULL; if (!p) TC_SKIP("not exported"); mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0;
    r = p(&out, F, m, fp);
    CHECK_EQ_I(r, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();
    DeleteFileA(F);
}
TC_CASE(missing_file)
{
    FILE *fp, *out = NULL; int r; if (!p) TC_SKIP("not exported"); mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    r = p(&out, "tc_fio_neg_freopen_s_missing_" SFX ".tmp", "r", fp);
    CHECK_EQ_I(r, ENOENT);
    _fcloseall();
    DeleteFileA(F);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install()); p = (freopen_s_t)tc_crt_proc("freopen_s");
    TC_RUN(null_pfile); TC_RUN(null_stream); TC_RUN(null_mode); TC_RUN(missing_file);
    return tc_end();
}
