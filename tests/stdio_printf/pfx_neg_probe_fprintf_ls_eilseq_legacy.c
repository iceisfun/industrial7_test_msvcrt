/* TC
 * id:    pfx_neg_probe_fprintf_ls_eilseq_legacy
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   <140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf / printf format specification (%ls)
 * note:  PROBES the legacy CRT outcome of %ls with an unrepresentable wide char in the C locale: either -1 (expected here) or a
 *        substituted '?' (-> "a?b", 3). returns_minus1 pins -1 (the more likely outcome, UNVERIFIED: a legacy CRT might
 *        instead emit a substitute char); errno_eilseq asserts EILSEQ only if the call failed. Either may fail on real Windows.
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

#define TMP "tc_pfx_neg_probe_fprintf_ls_eilseq_legacy_" TCV ".tmp"
static const wchar_t *volatile g_w = L"\x4e2d";

TC_CASE(returns_minus1)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    errno = 0; r = fprintf(f, "a%lsb", g_w);
    CHECK_EQ_I(r, -1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(errno_eilseq)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    errno = 0; r = fprintf(f, "a%lsb", g_w);
    if (r < 0) CHECK_EQ_I(errno, EILSEQ);
    fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(returns_minus1); TC_RUN(errno_eilseq);
    return tc_end();
}
