/* TC
 * id:    pfx_neg_probe_fprintf_ls_eilseq
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fprintf / printf format specification (%ls); errno EILSEQ
 * note:  PROBES that in the "C" locale %ls of L"\x4e2d" (not representable in the ANSI/C codepage) makes ucrt fprintf return -1
 *        with errno EILSEQ and WITHOUT calling the invalid-parameter handler, leaving the stream usable. Legacy CRTs: see
 *        pfx_neg_probe_fprintf_ls_eilseq_legacy.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
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

#define TMP "tc_pfx_neg_probe_fprintf_ls_eilseq_" TCV ".tmp"
static const wchar_t *volatile g_w = L"\x4e2d";

TC_CASE(eilseq)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, "a%lsb", g_w);
    CHECK(r < 0); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(usable_and_no_trailing)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "a%lsb", g_w); CHECK(r < 0);
    r = fprintf(f, "ok"); CHECK_EQ_I(r, 2);
    CHECK_EQ_I(fclose(f), 0);
    rd(TMP, b, sizeof b);
    CHECK(tc_strequ(b, "ok") || tc_strequ(b, "aok"));   /* 'a' before the bad conversion may or may not be present; 'b' never */
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(eilseq); TC_RUN(usable_and_no_trailing);
    return tc_end();
}
