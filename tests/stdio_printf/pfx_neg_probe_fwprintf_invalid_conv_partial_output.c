/* TC
 * id:    pfx_neg_probe_fwprintf_invalid_conv_partial_output
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf format specification (wide variants); "Parameter validation"
 * note:  PROBES the partial output after fwprintf(L"abc%yxyz") fails on an invalid conversion: file (ANSI "w" stream, so narrow bytes)
 *        is expected to contain exactly "abc" after fclose, and the stream stays usable for a following fwprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
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

#define TMP "tc_pfx_neg_probe_fwprintf_invalid_conv_partial_output_" TCV ".tmp"
static const wchar_t *volatile g_fmt = L"abc%yxyz";

TC_CASE(fail_codes)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fwprintf(f, g_fmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(partial_and_reuse)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, g_fmt, 1); CHECK_EQ_I(r, -1);
    r = fwprintf(f, L"Z"); CHECK_EQ_I(r, 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "abcZ");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(fail_codes); TC_RUN(partial_and_reuse);
    return tc_end();
}
