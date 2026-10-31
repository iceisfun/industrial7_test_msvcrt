/* TC
 * id:    chr_getw_minus_one_ambiguity
 * area:  stdio_char
 * funcs: _getw _putw fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getw: 'use feof or ferror to distinguish'
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

TC_CASE(minus_one_data_vs_eof)
{
    FILE *f; static const unsigned char m1[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    put_raw(FN, m1, 4);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), -1);
    CHECK(feof(f) == 0);
    CHECK(ferror(f) == 0);
    CHECK_EQ_I(_getw(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(putw_minus_one)
{
    FILE *f; volatile int v = -1; unsigned char raw[8]; DWORD n;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_putw(v, f), -1);
    CHECK(ferror(f) == 0);      /* -1 return here is data, not an error */
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 4);
    if (n == 4) { CHECK_EQ_U(raw[0], 0xFF); CHECK_EQ_U(raw[3], 0xFF); }
    DeleteFileA(FN);
}
TC_CASE(disambiguate_loop)
{
    FILE *f; int w, count = 0; static const unsigned char d[12] = { 1,0,0,0, 0xFF,0xFF,0xFF,0xFF, 2,0,0,0 };
    int got[4];
    put_raw(FN, d, 12);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    for (;;) {
        w = _getw(f);
        if (w == EOF && (feof(f) || ferror(f))) break;
        if (count < 4) got[count] = w;
        count++;
        if (count > 8) break;
    }
    CHECK_EQ_I(count, 3);
    if (count == 3) { CHECK_EQ_I(got[0], 1); CHECK_EQ_I(got[1], -1); CHECK_EQ_I(got[2], 2); }
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(minus_one_data_vs_eof); TC_RUN(putw_minus_one); TC_RUN(disambiguate_loop); return tc_end(); }
