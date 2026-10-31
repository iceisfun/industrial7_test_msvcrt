/* TC
 * id:    chr_neg_getw_short_and_empty
 * area:  stdio_char
 * kind:  negative
 * funcs: _getw fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getw: returns EOF on error or end of file
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

TC_CASE(empty_file)
{
    FILE *f; put_raw(FN, "", 0);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(short_file_three_bytes)
{
    FILE *f; static const unsigned char d[3] = { 1, 2, 3 };
    put_raw(FN, d, 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(eof_sticky)
{
    FILE *f; put_raw(FN, "", 0);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), EOF);
    CHECK_EQ_I(_getw(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(empty_file); TC_RUN(short_file_three_bytes); TC_RUN(eof_sticky); return tc_end(); }
