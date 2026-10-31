/* TC
 * id:    chr_probe_putw_text_expansion
 * area:  stdio_char
 * funcs: _putw _getw fopen
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putw/_getw (silent on text mode); fwrite/_write text translation
 * note:  PROBE: assumes _putw/_getw go through the text-mode LF<->CRLF translation (they are built from putc/getc in the CRT). Probes exactly that and nothing else.
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

TC_CASE(text_putw_expands_lf)
{
    FILE *f; volatile int v = 0x0A; unsigned char raw[16]; DWORD n;
    static const unsigned char want[5] = { 0x0D, 0x0A, 0, 0, 0 };
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_putw(v, f), 0x0A);
    fclose(f);
    n = get_raw(FN, raw, sizeof raw);
    CHECK_EQ_U(n, 5);
    if (n == 5) CHECK_MEM(raw, want, 5);
    DeleteFileA(FN);
}
TC_CASE(text_getw_collapses_crlf)
{
    FILE *f; static const unsigned char d[5] = { 0x0D, 0x0A, 0, 0, 0 };
    put_raw(FN, d, 5);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), 10);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(text_roundtrip)
{
    FILE *f; volatile int v = 0x0A;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    _putw(v, f); fclose(f);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(_getw(f), 10);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(text_putw_expands_lf); TC_RUN(text_getw_collapses_crlf); TC_RUN(text_roundtrip); return tc_end(); }
