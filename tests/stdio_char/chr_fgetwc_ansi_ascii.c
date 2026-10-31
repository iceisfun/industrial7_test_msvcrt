/* TC
 * id:    chr_fgetwc_ansi_ascii
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fgetc, fgetwc; C11 7.29.3.1; WEOF is 0xFFFF in wchar.h
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

TC_CASE(ascii_bytes)
{
    FILE *f; wint_t c;
    put_raw(FN, "Az9 ~", 5);
    f = fopen(FN, "rt"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'A');
    c = fgetwc(f); CHECK_EQ_U(c, 'z');
    c = fgetwc(f); CHECK_EQ_U(c, '9');
    c = fgetwc(f); CHECK_EQ_U(c, ' ');
    c = fgetwc(f); CHECK_EQ_U(c, '~');
    CHECK(feof(f) == 0);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK_EQ_U(WEOF, 0xFFFF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(text_crlf_to_lf)
{
    FILE *f; wint_t c;
    put_raw(FN, "a\r\nb\n", 5);
    f = fopen(FN, "rt"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'a');
    c = fgetwc(f); CHECK_EQ_U(c, L'\n');
    c = fgetwc(f); CHECK_EQ_U(c, 'b');
    c = fgetwc(f); CHECK_EQ_U(c, L'\n');
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(binary_reads_wchar_units)
{
    /* MS docs (Unicode stream I/O): in binary mode a wide-char input function reads whole wchar_t units, no translation */
    FILE *f; wint_t c; static const unsigned char d[] = { 'a', 0, '\r', 0, '\n', 0 };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'a');
    c = fgetwc(f); CHECK_EQ_U(c, L'\r');
    c = fgetwc(f); CHECK_EQ_U(c, L'\n');
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(empty_file)
{
    FILE *f; wint_t c;
    put_raw(FN, "", 0);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(feof(f) == 0);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    CHECK(feof(f) != 0);
    CHECK(ferror(f) == 0);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(ascii_bytes); TC_RUN(text_crlf_to_lf); TC_RUN(binary_reads_wchar_units); TC_RUN(empty_file); return tc_end(); }
