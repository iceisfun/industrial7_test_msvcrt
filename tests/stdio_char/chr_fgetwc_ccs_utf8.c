/* TC
 * id:    chr_fgetwc_ccs_utf8
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-8), fgetwc
 * note:  BMP only; no surrogate pairs asserted.
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

TC_CASE(bom_skipped_and_multibyte)
{
    FILE *f; wint_t c;
    static const unsigned char d[] = { 0xEF, 0xBB, 0xBF, 'a', 0xC3, 0xA9, 0xE2, 0x82, 0xAC, 'z' };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'a');
    c = fgetwc(f); CHECK_EQ_U(c, 0x00E9);
    c = fgetwc(f); CHECK_EQ_U(c, 0x20AC);
    c = fgetwc(f); CHECK_EQ_U(c, 'z');
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(crlf_to_lf)
{
    FILE *f; wint_t c;
    static const unsigned char d[] = { 0xEF, 0xBB, 0xBF, 'x', 0x0D, 0x0A, 'y', 0x0A };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'x');
    c = fgetwc(f); CHECK_EQ_U(c, L'\n');
    c = fgetwc(f); CHECK_EQ_U(c, 'y');
    c = fgetwc(f); CHECK_EQ_U(c, L'\n');
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(no_bom_still_decodes)
{
    FILE *f; wint_t c;
    static const unsigned char d[] = { 0xC3, 0xA9, 'q', 0xE2, 0x82, 0xAC };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 0x00E9);
    c = fgetwc(f); CHECK_EQ_U(c, 'q');
    c = fgetwc(f); CHECK_EQ_U(c, 0x20AC);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(bom_skipped_and_multibyte); TC_RUN(crlf_to_lf); TC_RUN(no_bom_still_decodes); return tc_end(); }
