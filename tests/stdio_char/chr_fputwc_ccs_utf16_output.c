/* TC
 * id:    chr_fputwc_ccs_utf16_output
 * area:  stdio_char
 * funcs: fputwc fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (BOM written when creating a ccs file), fputwc
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

/* raw file helpers: kernel32 only, so the CRT under test never touches the fixture bytes */
static void put_raw(const char *n, const void *d, DWORD len)
{
    DWORD w; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (len) WriteFile(h, d, len, &w, NULL);
    CloseHandle(h);
}
static DWORD get_raw(const char *n, unsigned char *b, DWORD max)
{
    DWORD r = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0xFFFFFFFFu;
    if (!ReadFile(h, b, max, &r, NULL)) r = 0xFFFFFFFEu;
    CloseHandle(h);
    return r;
}
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_" TC_TEST_ID "_" TC_S(TC_CRT_VER) ".tmp"

TC_CASE(utf16le_bytes)
{
    FILE *f; unsigned char raw[32]; DWORD n; volatile wint_t a = L'A', s = 0x263A, nl = L'\n';
    static const unsigned char want[] = { 0xFF, 0xFE, 0x41, 0x00, 0x3A, 0x26, 0x0D, 0x00, 0x0A, 0x00 };
    f = fopen(FN, "wt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(a, f), L'A');
    CHECK_EQ_U(fputwc(s, f), 0x263A);
    CHECK_EQ_U(fputwc(nl, f), L'\n');
    CHECK_EQ_I(fclose(f), 0);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, sizeof want);
    if (n == sizeof want) CHECK_MEM(raw, want, sizeof want);
    DeleteFileA(FN);
}
TC_CASE(empty_file_has_only_bom)
{
    FILE *f; unsigned char raw[8]; DWORD n;
    f = fopen(FN, "wt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 2);
    if (n == 2) { CHECK_EQ_U(raw[0], 0xFF); CHECK_EQ_U(raw[1], 0xFE); }
    DeleteFileA(FN);
}
TC_CASE(roundtrip_via_fgetwc)
{
    FILE *f; wint_t c; volatile wint_t v = 0xFFFD;
    f = fopen(FN, "wt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    fputwc(v, f); fputwc(0x00E9, f); fclose(f);
    f = fopen(FN, "rt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 0xFFFD);
    c = fgetwc(f); CHECK_EQ_U(c, 0x00E9);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(utf16le_bytes); TC_RUN(empty_file_has_only_bom); TC_RUN(roundtrip_via_fgetwc); return tc_end(); }
