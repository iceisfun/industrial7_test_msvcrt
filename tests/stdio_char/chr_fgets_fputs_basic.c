/* TC
 * id:    chr_fgets_fputs_basic
 * area:  stdio_char
 * funcs: fgets fputs fopen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2 fgets, 7.21.7.4 fputs; MS docs fgets, fputs
 */
#include "tc.h"
#include <stdio.h>

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

TC_CASE(fgets_lines_and_limit)
{
    FILE *f; char b[16];
    put_raw(FN, "abcdefg\nxy\nlast", 15);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 4, f) == b); CHECK_STR(b, "abc");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "defg\n");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "xy\n");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "last");
    CHECK(feof(f) != 0);
    CHECK(fgets(b, 16, f) == NULL);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(fgets_text_crlf)
{
    FILE *f; char b[16];
    put_raw(FN, "one\r\ntwo\r\n", 10);
    f = fopen(FN, "rt"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "one\n");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "two\n");
    CHECK(fgets(b, 16, f) == NULL);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(fputs_no_newline_added)
{
    FILE *f; unsigned char raw[16]; DWORD n; volatile const char *s = "ab\ncd";
    f = fopen(FN, "wt"); CHECK(f != NULL); if (!f) return;
    CHECK(fputs((const char *)s, f) >= 0);
    CHECK(fputs("", f) >= 0);
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 6);
    if (n == 6) CHECK_MEM(raw, "ab\r\ncd", 6);
    DeleteFileA(FN);
}
TC_CASE(fputc_returns_char)
{
    FILE *f; unsigned char raw[4]; DWORD n; volatile int c = 0x1FF;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputc(c, f), 0xFF);   /* converted to unsigned char */
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 1);
    if (n == 1) CHECK_EQ_U(raw[0], 0xFF);
    DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(fgets_lines_and_limit); TC_RUN(fgets_text_crlf); TC_RUN(fputs_no_newline_added); TC_RUN(fputc_returns_char); return tc_end(); }
