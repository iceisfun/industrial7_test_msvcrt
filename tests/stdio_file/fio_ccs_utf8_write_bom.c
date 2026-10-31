/* TC
 * id:    fio_ccs_utf8_write_bom
 * area:  stdio_file
 * funcs: fopen fclose fputwc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-8: BOM written when a new file is opened for writing); fputwc
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_ccs_utf8_write_bom_" TC_VS ".tmp";
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
TC_CASE(empty_bom_only)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 3);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    DeleteFileA(NAME);
}
TC_CASE(chars)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputwc(L'A', f), L'A');
    CHECK_EQ_I(fputwc((wchar_t)0xE9, f), 0xE9);
    CHECK_EQ_I(fputwc((wchar_t)0x20AC, f), 0x20AC);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 9);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    CHECK_EQ_U(b[3], 0x41);
    CHECK_EQ_U(b[4], 0xC3); CHECK_EQ_U(b[5], 0xA9);
    CHECK_EQ_U(b[6], 0xE2); CHECK_EQ_U(b[7], 0x82); CHECK_EQ_U(b[8], 0xAC);
    DeleteFileA(NAME);
}
TC_CASE(euro_bytes)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputwc((wchar_t)0x20AC, f), 0x20AC);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 6);
    CHECK_EQ_U(b[3], 0xE2); CHECK_EQ_U(b[4], 0x82); CHECK_EQ_U(b[5], 0xAC);
    DeleteFileA(NAME);
}
int main(void)
{
    tc_begin(); DeleteFileA(NAME);
    TC_RUN(empty_bom_only); TC_RUN(chars); TC_RUN(euro_bytes);
    return tc_end();
}
