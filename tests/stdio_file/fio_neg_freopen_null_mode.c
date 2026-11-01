/* TC
 * id:    fio_neg_freopen_null_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen fopen _fcloseall
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs freopen (invalid NULL mode -> invalid parameter handler, EINVAL)
 * note:  TRIAGE win1093[34])?#934, class A): case name_and_mode_null on ucrtbase 10.0.19045: freopen(NULL, NULL, fp) returns NULL with errno EBADF (9) and does NOT
 *        call the invalid-parameter handler (observed got=9, iph 0); case mode_null (name given, mode NULL) still sees EINVAL + handler. Old expectation kept for non-UCRT.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
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

#define F "tc_fio_neg_freopen_mode_" SFX ".tmp"

TC_CASE(mode_null)
{
    FILE *fp; FILE *volatile r; const char *volatile m = NULL; mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = freopen(F, m, fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();  /* stream may or may not have been closed by the failed call */
    DeleteFileA(F);
}
TC_CASE(name_and_mode_null)
{
    FILE *fp; FILE *volatile r; const char *volatile m = NULL; const char *volatile nm = NULL; mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = freopen(nm, m, fp);
    CHECK(r == NULL);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, EBADF); CHECK_EQ_I(tc_iph_count, 0);   /* observed ucrtbase 10.0.19045: no handler call */
#else
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
#endif
    _fcloseall();  /* stream may or may not have been closed by the failed call */
    DeleteFileA(F);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(mode_null); TC_RUN(name_and_mode_null);
    return tc_end();
}
