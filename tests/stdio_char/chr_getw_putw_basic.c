/* TC
 * id:    chr_getw_putw_basic
 * area:  stdio_char
 * funcs: _putw _getw fopen
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getw, _putw
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

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

static const int vals[] = { 0, 1, INT_MAX, INT_MIN, 0x01020304, -5 };
#define NV ((int)(sizeof vals / sizeof vals[0]))

TC_CASE(putw_returns_value_and_bytes)
{
    FILE *f; int i; unsigned char raw[64]; DWORD n;
    static const unsigned char want[] = { 0,0,0,0, 1,0,0,0, 0xFF,0xFF,0xFF,0x7F, 0,0,0,0x80, 4,3,2,1, 0xFB,0xFF,0xFF,0xFF };
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    for (i = 0; i < NV; i++) { volatile int v = vals[i]; CHECK_EQ_I(_putw(v, f), vals[i]); }
    CHECK_EQ_I(fclose(f), 0);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 4 * NV);
    if (n == 4 * NV) CHECK_MEM(raw, want, sizeof want);
    DeleteFileA(FN);
}
TC_CASE(getw_roundtrip)
{
    FILE *f; int i;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    for (i = 0; i < NV; i++) { volatile int v = vals[i]; _putw(v, f); }
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    for (i = 0; i < NV; i++) CHECK_EQ_I(_getw(f), vals[i]);
    CHECK(feof(f) == 0);
    CHECK(ferror(f) == 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(minus_five_no_eof_flag)
{
    FILE *f; volatile int v = -5;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_putw(v, f), -5);
    fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), -5);
    CHECK(feof(f) == 0); CHECK(ferror(f) == 0);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(putw_returns_value_and_bytes); TC_RUN(getw_roundtrip); TC_RUN(minus_five_no_eof_flag); return tc_end(); }
