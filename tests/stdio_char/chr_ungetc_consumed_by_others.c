/* TC
 * id:    chr_ungetc_consumed_by_others
 * area:  stdio_char
 * funcs: ungetc fgets fopen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 ungetc; MS docs ungetc
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

TC_CASE(fgets_sees_pushback)
{
    FILE *f; char b[32]; volatile int c;
    put_raw(FN, "hello\nworld\n", 12);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    c = fgetc(f); CHECK_EQ_I(c, 'h');
    CHECK_EQ_I(ungetc('X', f), 'X');
    CHECK(fgets(b, sizeof b, f) == b);
    CHECK_STR(b, "Xello\n");
    CHECK(fgets(b, sizeof b, f) == b);
    CHECK_STR(b, "world\n");
    fclose(f); DeleteFileA(FN);
}
TC_CASE(fread_sees_pushback)
{
    FILE *f; char b[8] = {0}; volatile int c;
    put_raw(FN, "abcdef", 6);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    c = fgetc(f); CHECK_EQ_I(c, 'a');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_U(fread(b, 1, 2, f), 2);
    CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[1], 'b');
    CHECK_EQ_U(fread(b, 1, 8, f), 4);
    CHECK_EQ_I(b[0], 'c'); CHECK_EQ_I(b[3], 'f');
    CHECK(feof(f));
    fclose(f); DeleteFileA(FN);
}
TC_CASE(pushback_after_eof_clears_eof)
{
    FILE *f; volatile int c;
    put_raw(FN, "q", 1);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    c = fgetc(f); CHECK_EQ_I(c, 'q');
    c = fgetc(f); CHECK_EQ_I(c, EOF); CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc('q', f), 'q');
    CHECK(feof(f) == 0);       /* C11 7.21.7.10p5: a successful ungetc clears the EOF indicator */
    c = fgetc(f); CHECK_EQ_I(c, 'q');
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(fgets_sees_pushback); TC_RUN(fread_sees_pushback); TC_RUN(pushback_after_eof_clears_eof); return tc_end(); }
