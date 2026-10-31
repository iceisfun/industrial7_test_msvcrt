/* TC
 * id:    fio_neg_freopen_bad_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen fopen _fcloseall
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs freopen, fopen (mode strings); an unknown mode letter is not spelled out as a validation failure in the docs
 * note:  Mode "z" is rejected with EINVAL and the invalid parameter handler on msvcr80+. The stream may already have been closed
 *        by the failed call (unspecified), so it is never touched afterwards except via _fcloseall().
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

TC_CASE(mode_z)
{
    FILE *fp; FILE *volatile r; const char *volatile m = "z"; mk(F, "abc");
    fp = fopen(F, "r"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = freopen(F, m, fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();
    DeleteFileA(F);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(mode_z);
    return tc_end();
}
