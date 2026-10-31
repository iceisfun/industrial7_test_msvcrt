/* TC
 * id:    fio_probe_ccs_mode_syntax
 * area:  stdio_file
 * funcs: fopen fclose fputwc
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs syntax is documented only as "w, ccs=UNICODE")
 * note:  PROBE: (1) mode string with no space after the comma ("w,ccs=UNICODE") is accepted and behaves like
 *        "w, ccs=UNICODE"; (2) "wb, ccs=UTF-8" (explicit b with ccs) still writes the UTF-8 BOM.
 *        Neither form is documented.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_probe_ccs_mode_syntax_" TC_VS ".tmp";
static int rawread(const char *n, unsigned char *b, int max)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)max, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return (int)got;
}
TC_CASE(no_space_after_comma)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w,ccs=UNICODE");
    CHECK(f != NULL); if (!f) return;
    fputwc((wchar_t)0x4E2D, f); fputwc(L'Z', f); fclose(f);
    CHECK_EQ_I(rawread(NAME, b, 16), 6);
    CHECK_EQ_U(b[0], 0xFF); CHECK_EQ_U(b[1], 0xFE); CHECK_EQ_U(b[2], 0x2D);
    CHECK_EQ_U(b[3], 0x4E); CHECK_EQ_U(b[4], 0x5A); CHECK_EQ_U(b[5], 0);
    DeleteFileA(NAME);
}
TC_CASE(wb_with_ccs)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "wb, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    fclose(f);
    CHECK_EQ_I(rawread(NAME, b, 16), 3);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(no_space_after_comma); TC_RUN(wb_with_ccs); return tc_end(); }
