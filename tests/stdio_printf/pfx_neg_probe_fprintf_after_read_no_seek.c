/* TC
 * id:    pfx_neg_probe_fprintf_after_read_no_seek
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fgetc fopen fclose ferror
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.5.3p7 (output after input without intervening fflush/seek is undefined unless EOF was hit)
 * note:  OBSERVED on ucrtbase (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "write_after_read_fails r < 0", "ferror(f) != 0"; zzp_probe4 "fprintf after read: r=1 ferror=0 file=[aX]"): on a "w+" stream
 *        holding one byte, fgetc consumes it and fprintf with no fseek/fflush in between SUCCEEDS (r=1, no error flag) and the file ends up as "aX".
 *        The EOF/_IOERR failure belief is kept for pre-UCRT CRTs (unverified). C11 7.21.5.3p7: output after input without an intervening
 *        fflush/file-positioning call is undefined, so ucrt is conforming. Stable.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
static int rd(const char *p, char *b, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); DWORD got = 0;
    if (h == INVALID_HANDLE_VALUE) { b[0] = 0; return -1; }
    ReadFile(h, b, (DWORD)(n - 1), &got, NULL); CloseHandle(h); b[got] = 0; return (int)got;
}
static void wr(const char *p, const char *d, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, (DWORD)n, &w, NULL); CloseHandle(h); }
}

#define TMP "tc_pfx_neg_probe_fprintf_after_read_no_seek_" TCV ".tmp"

TC_CASE(write_after_read_fails)
{
    FILE *f = fopen(TMP, "w+"); int r; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a"), 1);
    rewind(f);
    CHECK_EQ_I(fgetc(f), 'a');
    r = fprintf(f, "X");
#if TC_CRT_VER >= 140
    CHECK_EQ_I(r, 1);            /* ucrtbase observed: the write is accepted */
    CHECK_EQ_I(ferror(f), 0);
    CHECK_EQ_I(fclose(f), 0);
    { char b[8]; CHECK_EQ_I(rd(TMP, b, sizeof b), 2); CHECK_STR(b, "aX"); }
    DeleteFileA(TMP);
#else
    CHECK(r < 0);
    CHECK(ferror(f) != 0);
    fclose(f); DeleteFileA(TMP);
#endif
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(write_after_read_fails);
    return tc_end();
}
