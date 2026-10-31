/* TC
 * id:    pfx_neg_probe_fprintf_after_read_no_seek
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fgetc fopen fclose ferror
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.5.3p7 (output after input without intervening fflush/seek is undefined unless EOF was hit)
 * note:  PROBES the CRT behaviour (undefined by the standard) of a "w+" stream holding exactly one byte: fgetc consumes it (read buffer
 *        now exhausted, EOF not yet seen) then fprintf with no fseek/fflush. Expected: the buffer-full path (_flsbuf) sees _IOREAD without
 *        _IOEOF, returns EOF and sets _IOERR, so fprintf is negative and ferror != 0. (A 1-byte file is used on purpose: with unread bytes
 *        left in the read buffer the byte would just be stored in the buffer and fprintf would not fail.)
 *        The well-defined cases (with fseek, or after EOF) are in pfx_fprintf_read_then_write_seek.
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
    CHECK(r < 0);
    CHECK(ferror(f) != 0);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(write_after_read_fails);
    return tc_end();
}
