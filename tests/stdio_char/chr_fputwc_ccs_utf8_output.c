/* TC
 * id:    chr_fputwc_ccs_utf8_output
 * area:  stdio_char
 * funcs: fputwc fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-8 writes BOM), fputwc
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

TC_CASE(utf8_bytes)
{
    FILE *f; unsigned char raw[32]; DWORD n; volatile wint_t e = 0xE9, eur = 0x20AC, nl = L'\n', a = L'k';
    static const unsigned char want[] = { 0xEF, 0xBB, 0xBF, 0xC3, 0xA9, 0xE2, 0x82, 0xAC, 0x0D, 0x0A, 'k' };
    f = fopen(FN, "wt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(e, f), 0xE9);
    CHECK_EQ_U(fputwc(eur, f), 0x20AC);
    CHECK_EQ_U(fputwc(nl, f), L'\n');
    CHECK_EQ_U(fputwc(a, f), L'k');
    CHECK_EQ_I(fclose(f), 0);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, sizeof want);
    if (n == sizeof want) CHECK_MEM(raw, want, sizeof want);
    DeleteFileA(FN);
}
TC_CASE(empty_file_has_only_bom)
{
    FILE *f; unsigned char raw[8]; DWORD n;
    f = fopen(FN, "wt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 3);
    if (n == 3) { CHECK_EQ_U(raw[0], 0xEF); CHECK_EQ_U(raw[1], 0xBB); CHECK_EQ_U(raw[2], 0xBF); }
    DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(utf8_bytes); TC_RUN(empty_file_has_only_bom); return tc_end(); }
