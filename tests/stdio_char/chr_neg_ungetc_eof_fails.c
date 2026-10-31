/* TC
 * id:    chr_neg_ungetc_eof_fails
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fgetc fopen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10p4: if c equals EOF the operation fails and the stream is unchanged; MS docs ungetc
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

TC_CASE(ungetc_eof)
{
    FILE *f; volatile int e = EOF;
    put_raw(FN, "abc", 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc(e, f), EOF);
    CHECK_EQ_I(fgetc(f), 'b');   /* stream unchanged by the failed pushback */
    CHECK_EQ_I(fgetc(f), 'c');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(ungetc_eof_on_fresh_stream)
{
    FILE *f; volatile int e = EOF;
    put_raw(FN, "abc", 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc(e, f), EOF);
    CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(ungetc_eof); TC_RUN(ungetc_eof_on_fresh_stream); return tc_end(); }
