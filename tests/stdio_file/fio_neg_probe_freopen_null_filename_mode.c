/* TC
 * id:    fio_neg_probe_freopen_null_filename_mode
 * area:  stdio_file
 * kind:  negative
 * funcs: freopen fopen _fcloseall
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.4 (NULL filename = mode change, implementation-defined); MS docs freopen
 * note:  Probes: the Windows CRT is believed NOT to support freopen(NULL, mode, fp) (commonly reported: it returns NULL with
 *        EINVAL via the invalid parameter handler, so "freopen(NULL,"wb",stdout)" cannot be used to switch to binary mode).
 *        Asserted for both "rb" and "wb". An invalid parameter handler is installed so the probe cannot be fatal.
 *        Limited to >=80 because msvcrt.dll has no parameter validation here (a NULL filename could crash it).
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

#define F "tc_fio_neg_probe_freopen_null_" SFX ".tmp"

TC_CASE(null_to_read)
{
    FILE *fp; FILE *volatile r; const char *volatile nm = NULL; mk(F, "abc123");
    fp = fopen(F, "r+"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = freopen(nm, "rb", fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();  /* fp may or may not have been closed */
    DeleteFileA(F);
}
TC_CASE(null_to_write)
{
    FILE *fp; FILE *volatile r; const char *volatile nm = NULL; char out[16]; mk(F, "abc123");
    fp = fopen(F, "r+"); CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = freopen(nm, "wb", fp);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    _fcloseall();
    CHECK_EQ_I(rd(F, out, sizeof out), 6);  /* file must not have been truncated by the failed call */
    DeleteFileA(F);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_to_read); TC_RUN(null_to_write); return tc_end(); }
