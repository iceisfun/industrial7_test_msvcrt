/* TC
 * id:    fio_neg_freopen_missing_file
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.5.4 (original stream is closed; returns null pointer on failure); MS docs freopen (original file closed, returns NULL); errno.h docs ENOENT
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

TC_CASE(missing)
{
    FILE *fp; FILE *volatile r; const char *volatile name = "tc_fio_neg_freopen_missing_" SFX ".tmp";
    mk("tc_fio_neg_freopen_exist_" SFX ".tmp", "abc");
    DeleteFileA("tc_fio_neg_freopen_missing_" SFX ".tmp");
    fp = fopen("tc_fio_neg_freopen_exist_" SFX ".tmp", "r"); CHECK(fp != NULL); if (!fp) return;
    errno = 0;
    r = freopen(name, "r", fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOENT);
    /* fp is closed by freopen; do not touch it */
    DeleteFileA("tc_fio_neg_freopen_exist_" SFX ".tmp");
}
int main(void) { tc_begin(); TC_RUN(missing); return tc_end(); }
