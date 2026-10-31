/* TC
 * id:    pfw_fwprintf_ansi_text_crlf
 * area:  stdio_printf
 * funcs: fwprintf _wfopen
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fwprintf (wide output to a non-Unicode stream is converted to multibyte), fopen text/binary mode
 * note:  Only ASCII payloads are used so the result does not depend on the ANSI code page.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_ansi_" TC_S1(TC_CRT_VER) ".tmp"
#define WFN L"" FN
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
TC_CASE(text_mode_ascii_and_crlf_translation)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w"); int n, r;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"a%d\nb", 5); fclose(f);
    CHECK_EQ_I(r, 4);
    n = slurp(b, 64); CHECK_EQ_I(n, 5); CHECK(b[0] == 'a' && b[1] == '5' && b[2] == 0x0D && b[3] == 0x0A && b[4] == 'b');
    DeleteFileA(FN);
}
TC_CASE(binary_mode_no_crlf)
{
    /* In binary mode no LF->CRLF translation happens; whether the wide chars are narrowed or written raw is not asserted. */
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"wb"); int n, i, lf = 0;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"a\nb"); fclose(f);
    n = slurp(b, 64); CHECK(n >= 3);
    for (i = 0; i < n; i++) { CHECK(b[i] != 0x0D); if (b[i] == 0x0A) lf++; }
    CHECK_EQ_I(lf, 1);
    DeleteFileA(FN);
}
TC_CASE(wide_ascii_roundtrip_bytes)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w"); int n, i;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"%ls|%5.2f", L"wide", 1.5); fclose(f);
    n = slurp(b, 64); CHECK(n > 0);
    for (i = 0; i < n; i++) CHECK(b[i] != 0);   /* narrowed: no UTF-16 NUL bytes */
    CHECK_EQ_I(n, 10); CHECK(n >= 5 && b[0] == 'w' && b[1] == 'i' && b[2] == 'd' && b[3] == 'e');
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(text_mode_ascii_and_crlf_translation); TC_RUN(binary_mode_no_crlf); TC_RUN(wide_ascii_roundtrip_bytes);
    return tc_end();
}
