/* TC
 * id:    pfx_fwprintf_ccs_utf8
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (ccs=UTF-8 writes a BOM; Unicode streams translate); MS docs fwprintf
 * note:  ccs=UTF-8 stream: BOM EF BB BF, wide output transcoded to UTF-8, text-mode newline -> CR LF.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_fwprintf_ccs_utf8_a_" TC_CRT_DLL ".tmp"

TC_CASE(bom_and_ascii)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"hi");
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 5);
    CHECK_MEM(b, "\xEF\xBB\xBF" "hi", 5);
    remove(FN);
}
TC_CASE(non_ascii)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%lc%lc", (wint_t)0x00E9, (wint_t)0x20AC);
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 8);
    CHECK_MEM(b, "\xEF\xBB\xBF\xC3\xA9\xE2\x82\xAC", 8);
    remove(FN);
}
TC_CASE(newline_crlf)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"a\n");
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 6);
    CHECK_MEM(b, "\xEF\xBB\xBF" "a\r\n", 6);
    remove(FN);
}
TC_CASE(count_is_wide_chars)
{
    FILE *f = fopen(FN, "w, ccs=UTF-8"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"%lc%lc%lc", (wint_t)0x00E9, (wint_t)0x20AC, (wint_t)'x');
    CHECK_EQ_I(n, 3);   /* 3 wide chars, 6 bytes of UTF-8 */
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    TC_RUN(bom_and_ascii); TC_RUN(non_ascii); TC_RUN(newline_crlf); TC_RUN(count_is_wide_chars);
    return tc_end();
}
