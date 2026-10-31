/* TC
 * id:    fio_probe_ccs_utf16_text_crlf
 * area:  stdio_file
 * funcs: fopen fclose fputwc fgetwc
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs implies text mode)
 * note:  PROBE: newline translation in ccs text mode. Writing L'\n' with ccs=UTF-16LE is expected to
 *        produce 0D 00 0A 00; reading 0D 00 0A 00 back is expected to yield a single L'\n'.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_probe_ccs_utf16_text_crlf_" TC_VS ".tmp";
static int rawread(const char *n, unsigned char *b, int max)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)max, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return (int)got;
}
static void rawwrite(const char *n, const void *b, int len)
{
    DWORD put; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, b, (DWORD)len, &put, NULL); CloseHandle(h);
}
TC_CASE(write_crlf)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-16LE");
    CHECK(f != NULL); if (!f) return;
    fputwc(L'\n', f); fclose(f);
    CHECK_EQ_I(rawread(NAME, b, 16), 6);
    CHECK_EQ_U(b[2], 0x0D); CHECK_EQ_U(b[3], 0); CHECK_EQ_U(b[4], 0x0A); CHECK_EQ_U(b[5], 0);
    DeleteFileA(NAME);
}
TC_CASE(read_crlf)
{
    static const unsigned char d[] = { 0xFF, 0xFE, 0x0D, 0x00, 0x0A, 0x00, 0x41, 0x00 };
    FILE *f; rawwrite(NAME, d, sizeof d);
    f = fopen(NAME, "r, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    CHECK_EQ_I(fgetwc(f), L'\n');
    CHECK_EQ_I(fgetwc(f), 0x41);
    CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(write_crlf); TC_RUN(read_crlf); return tc_end(); }
