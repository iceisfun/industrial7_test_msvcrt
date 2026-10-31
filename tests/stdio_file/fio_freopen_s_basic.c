/* TC
 * id:    fio_freopen_s_basic
 * area:  stdio_file
 * funcs: fopen fclose fread
 * procs: freopen_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs freopen_s, _wfreopen_s (returns 0 on success)
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

#define A "tc_fio_freopen_s_a_" SFX ".tmp"
#define B "tc_fio_freopen_s_b_" SFX ".tmp"

TC_CASE(success)
{
    FILE *fp, *out = NULL; char buf[16]; size_t n; int r; if (!p) TC_SKIP("freopen_s not exported");
    mk(A, "AAAA"); mk(B, "BBBBBB");
    fp = fopen(A, "r"); CHECK(fp != NULL); if (!fp) return;
    r = p(&out, B, "r", fp);
    CHECK_EQ_I(r, 0); CHECK(out != NULL);
    if (r == 0 && out) {
        n = fread(buf, 1, sizeof buf - 1, out); buf[n] = 0;
        CHECK_EQ_U(n, 6); CHECK_STR(buf, "BBBBBB");
        fclose(out);
    }
    DeleteFileA(A); DeleteFileA(B);
}
TC_CASE(std_streams_untouched)
{
    FILE *fp, *out = NULL; int r; if (!p) TC_SKIP("freopen_s not exported");
    mk(A, "AAAA"); mk(B, "BBBBBB");
    fp = fopen(A, "r"); CHECK(fp != NULL); if (!fp) return;
    r = p(&out, B, "r", fp);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(ferror(stdout), 0);
    CHECK_EQ_I(ferror(stdin), 0);
    CHECK_EQ_I(ferror(stderr), 0);
    if (out) fclose(out);
    DeleteFileA(A); DeleteFileA(B);
}
int main(void)
{
    tc_begin(); p = (freopen_s_t)tc_crt_proc("freopen_s");
    TC_RUN(success); TC_RUN(std_streams_untouched);
    return tc_end();
}
