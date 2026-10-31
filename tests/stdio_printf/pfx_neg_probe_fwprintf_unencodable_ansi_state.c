/* TC
 * id:    pfx_neg_probe_fwprintf_unencodable_ansi_state
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fopen fclose ftell
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fwprintf (EILSEQ on conversion failure); C11 7.29.2.11
 * note:  Probes the stream state after fwprintf of L"ab<U+4E2D>cd" to an ANSI "w" stream in the C locale fails with
 *        EILSEQ: that exactly 2 bytes ("ab") reached the file, ftell agrees, and a later fwprintf works.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define FN "tc_pfx_neg_probe_fwprintf_unencodable_ansi_state.tmp"
static FILE *f;

static int readback(char *b, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, cap - 1, &n, NULL); CloseHandle(h); b[n] = 0; return (int)n;
}

TC_CASE(fails)
{
    int r; f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fwprintf(f, L"ab\x4e2d" L"cd");
    CHECK(r < 0);
    CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(state)
{
    char b[32]; int n;
    if (!f) TC_SKIP("no file");
    fflush(f);
    CHECK_EQ_I(ftell(f), 2);
    n = readback(b, sizeof b);
    CHECK_EQ_I(n, 2);
}
TC_CASE(later_ok)
{
    char b[32]; int r, n;
    if (!f) TC_SKIP("no file");
    r = fwprintf(f, L"ok");
    CHECK_EQ_I(r, 2);
    fclose(f); f = NULL;
    n = readback(b, sizeof b);
    CHECK_EQ_I(n, 4);
    CHECK_STR(b, "abok");
    DeleteFileA(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fails); TC_RUN(state); TC_RUN(later_ok); return tc_end(); }
