/* TC
 * id:    chr_fputwc_ansi_ascii
 * area:  stdio_char
 * funcs: fputwc fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fputc, fputwc
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

TC_CASE(returns_char_and_writes_bytes)
{
    FILE *f; unsigned char raw[16]; DWORD n; volatile wint_t a = L'A', t = L'~';
    f = fopen(FN, "wt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(a, f), L'A');
    CHECK_EQ_U(fputwc(t, f), L'~');
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 2);
    if (n == 2) { CHECK_EQ_U(raw[0], 'A'); CHECK_EQ_U(raw[1], '~'); }
    DeleteFileA(FN);
}
TC_CASE(newline_text)
{
    FILE *f; unsigned char raw[16]; DWORD n; volatile wint_t nl = L'\n';
    f = fopen(FN, "wt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(nl, f), L'\n');
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 2);
    if (n == 2) { CHECK_EQ_U(raw[0], 0x0D); CHECK_EQ_U(raw[1], 0x0A); }
    DeleteFileA(FN);
}
TC_CASE(newline_binary)
{
    /* MS docs: in binary mode fputwc writes the whole wchar_t unit, untranslated */
    FILE *f; unsigned char raw[16]; DWORD n; volatile wint_t nl = L'\n';
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(nl, f), L'\n');
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 2);
    if (n == 2) { CHECK_EQ_U(raw[0], 0x0A); CHECK_EQ_U(raw[1], 0x00); }
    DeleteFileA(FN);
}
TC_CASE(nul_char)
{
    FILE *f; unsigned char raw[16]; DWORD n; volatile wint_t z = 0;
    f = fopen(FN, "wt"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fputwc(z, f), 0);
    fputwc(L'x', f);
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 2);
    if (n == 2) { CHECK_EQ_U(raw[0], 0); CHECK_EQ_U(raw[1], 'x'); }
    DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(returns_char_and_writes_bytes); TC_RUN(newline_text); TC_RUN(newline_binary); TC_RUN(nul_char); return tc_end(); }
