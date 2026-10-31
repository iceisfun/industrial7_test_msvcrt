/* TC
 * id:    fio_ccs_utf16le_write_bom
 * area:  stdio_file
 * funcs: fopen fclose fputwc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-16LE / UNICODE); fputwc
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_ccs_utf16le_write_bom_" TC_VS ".tmp";
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
TC_CASE(bom_and_chars)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-16LE");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputwc(L'A', f), L'A');
    CHECK_EQ_I(fputwc((wchar_t)0x4E2D, f), 0x4E2D);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 6);
    CHECK_EQ_U(b[0], 0xFF); CHECK_EQ_U(b[1], 0xFE);
    CHECK_EQ_U(b[2], 0x41); CHECK_EQ_U(b[3], 0x00);
    CHECK_EQ_U(b[4], 0x2D); CHECK_EQ_U(b[5], 0x4E);
    DeleteFileA(NAME);
}
TC_CASE(surrogates)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-16LE");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputwc((wchar_t)0xD83D, f), 0xD83D);
    CHECK_EQ_I(fputwc((wchar_t)0xDE00, f), 0xDE00);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 6);
    CHECK_EQ_U(b[2], 0x3D); CHECK_EQ_U(b[3], 0xD8); CHECK_EQ_U(b[4], 0x00); CHECK_EQ_U(b[5], 0xDE);
    DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(bom_and_chars); TC_RUN(surrogates); return tc_end(); }
