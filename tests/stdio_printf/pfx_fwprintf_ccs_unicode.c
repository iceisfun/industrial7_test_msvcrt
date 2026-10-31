/* TC
 * id:    pfx_fwprintf_ccs_unicode
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UNICODE: UTF-16LE with BOM); MS docs fwprintf
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

#define FN "tc_pfx_fwprintf_ccs_unicode_a_" TC_CRT_DLL ".tmp"

TC_CASE(bom_and_units)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UNICODE"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"A%lc", (wint_t)0x20AC);
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 6);
    CHECK_MEM(b, "\xFF\xFE" "A\x00\xAC\x20", 6);
    remove(FN);
}
TC_CASE(newline_crlf)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UNICODE"); int n;
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, L"\n");
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 6);
    CHECK_MEM(b, "\xFF\xFE\x0D\x00\x0A\x00", 6);
    remove(FN);
}
TC_CASE(surrogate_pair)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w, ccs=UNICODE"); int n;
    static const wchar_t fmt[] = { 0xD83D, 0xDE00, 0 };   /* U+1F600 as two units */
    CHECK(f != NULL); if (!f) return;
    fwprintf(f, fmt);
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 6);
    CHECK_MEM(b, "\xFF\xFE\x3D\xD8\x00\xDE", 6);
    remove(FN);
}
TC_CASE(count_is_wide_chars)
{
    FILE *f = fopen(FN, "w, ccs=UNICODE"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"%d-%ls", 12, L"ab");
    CHECK_EQ_I(n, 5);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    TC_RUN(bom_and_units); TC_RUN(newline_crlf); TC_RUN(surrogate_pair); TC_RUN(count_is_wide_chars);
    return tc_end();
}
