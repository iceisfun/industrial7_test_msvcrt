/* TC
 * id:    chr_fgetwc_ccs_unicode_bom_detect
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen: 'If a BOM is present, it takes precedence over the encoding specified by the ccs flag'
 * note:  The no-BOM default is probed separately in chr_fgetwc_probe_ccs_unicode_nobom.
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

TC_CASE(utf16le_bom)
{
    FILE *f; wint_t c; static const unsigned char d[] = { 0xFF, 0xFE, 'h', 0, 0x3A, 0x26 };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UNICODE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'h');
    c = fgetwc(f); CHECK_EQ_U(c, 0x263A);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(utf8_bom)
{
    FILE *f; wint_t c; static const unsigned char d[] = { 0xEF, 0xBB, 0xBF, 'h', 0xC3, 0xA9, 0xE2, 0x82, 0xAC };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UNICODE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'h');
    c = fgetwc(f); CHECK_EQ_U(c, 0x00E9);
    c = fgetwc(f); CHECK_EQ_U(c, 0x20AC);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(bom_beats_explicit_ccs)
{
    FILE *f; wint_t c; static const unsigned char d[] = { 0xFF, 0xFE, 'h', 0 };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UTF-8"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'h');
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(utf16le_bom); TC_RUN(utf8_bom); TC_RUN(bom_beats_explicit_ccs); return tc_end(); }
