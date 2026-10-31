/* TC
 * id:    chr_ungetc_unsigned_char
 * area:  stdio_char
 * funcs: ungetc fgetc fopen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 (c converted to unsigned char); MS docs ungetc
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

static FILE *open_primed(void)
{
    FILE *f; volatile int c;
    put_raw(FN, "ab", 2);
    f = fopen(FN, "rb"); if (!f) return NULL;
    c = fgetc(f); (void)c;
    return f;
}
TC_CASE(ff_not_eof)
{
    FILE *f = open_primed(); volatile int c, v = 0xFF; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc(v, f), 255);
    c = fgetc(f); CHECK_EQ_I(c, 255);
    c = fgetc(f); CHECK_EQ_I(c, 'b');
    fclose(f); DeleteFileA(FN);
}
TC_CASE(truncated_to_unsigned_char)
{
    FILE *f = open_primed(); volatile int c, v = 0x141; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc(v, f), 0x41);
    c = fgetc(f); CHECK_EQ_I(c, 0x41);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(char_minus_one_is_255)
{
    FILE *f = open_primed(); volatile char ch = (char)-1; volatile int c; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc((int)(unsigned char)ch, f), 255);
    c = fgetc(f); CHECK_EQ_I(c, 255);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(ff_not_eof); TC_RUN(truncated_to_unsigned_char); TC_RUN(char_minus_one_is_255); return tc_end(); }
