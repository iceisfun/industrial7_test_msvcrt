/* TC
 * id:    chr_fgetwc_ccs_utf16
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UNICODE|UTF-8|UTF-16LE, BOM handling), fgetwc
 * note:  msvcrt.dll has no ccs support.
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

static const unsigned char file1[] = { 0xFF, 0xFE, 0x41, 0x00, 0x3A, 0x26, 0x0D, 0x00, 0x0A, 0x00, 0xFD, 0xFF, 0x00, 0x04, 0xE9, 0x00 };
static const unsigned short want1[] = { 'A', 0x263A, '\n', 0xFFFD, 0x0400, 0x00E9 };

static void check_stream(FILE *f)
{
    size_t i; wint_t c;
    for (i = 0; i < sizeof want1 / sizeof want1[0]; i++) { c = fgetwc(f); CHECK_EQ_U(c, want1[i]); }
    CHECK(feof(f) == 0);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
}
TC_CASE(utf16le_bom_skipped)
{
    FILE *f; wint_t c;
    put_raw(FN, file1, 6);
    f = fopen(FN, "rt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'A');
    c = fgetwc(f); CHECK_EQ_U(c, 0x263A);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(crlf_and_bmp_chars)
{
    FILE *f;
    put_raw(FN, file1, sizeof file1);
    f = fopen(FN, "rt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    check_stream(f);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(ccs_unicode_same_file)
{
    FILE *f;
    put_raw(FN, file1, sizeof file1);
    f = fopen(FN, "rt, ccs=UNICODE"); CHECK(f != NULL); if (!f) return;
    check_stream(f);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(utf16_file_without_bom)
{
    FILE *f; wint_t c;
    put_raw(FN, file1 + 2, 4);
    f = fopen(FN, "rt, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'A');
    c = fgetwc(f); CHECK_EQ_U(c, 0x263A);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(utf16le_bom_skipped); TC_RUN(crlf_and_bmp_chars); TC_RUN(ccs_unicode_same_file); TC_RUN(utf16_file_without_bom); return tc_end(); }
