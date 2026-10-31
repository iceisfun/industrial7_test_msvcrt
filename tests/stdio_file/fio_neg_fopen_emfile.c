/* TC
 * id:    fio_neg_fopen_emfile
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose _setmaxstdio _getmaxstdio
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen (EMFILE), _setmaxstdio
 * note:  Exact count of successful fopens is not asserted (depends on std streams). Only that exhaustion eventually fails with EMFILE.
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

TC_CASE(exhaust)
{
    FILE *fs[64]; int i, n = 0, e = 0; int r;
    mk("tc_fio_neg_emfile_" SFX ".tmp", "x");
    r = _setmaxstdio(32);
    CHECK_EQ_I(r, 32);
    for (i = 0; i < 64; i++) {
        errno = 0; fs[i] = fopen("tc_fio_neg_emfile_" SFX ".tmp", "r");
        if (!fs[i]) { e = errno; break; }
        n++;
    }
    CHECK(n >= 1);
    CHECK(n < 64);
    CHECK_EQ_I(e, EMFILE);
    for (i = 0; i < n; i++) fclose(fs[i]);
    r = _setmaxstdio(512);
    CHECK_EQ_I(r, 512);
    CHECK_EQ_I(_getmaxstdio(), 512);
    {   /* usable again */
        FILE *f = fopen("tc_fio_neg_emfile_" SFX ".tmp", "r"); CHECK(f != NULL); if (f) fclose(f);
    }
    DeleteFileA("tc_fio_neg_emfile_" SFX ".tmp");
}
int main(void) { tc_begin(); TC_RUN(exhaust); return tc_end(); }
