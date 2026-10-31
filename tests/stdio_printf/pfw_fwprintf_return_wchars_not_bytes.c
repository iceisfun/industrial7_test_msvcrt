/* TC
 * id:    pfw_fwprintf_return_wchars_not_bytes
 * area:  stdio_printf
 * funcs: fwprintf _wfopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf: "returns the number of wide characters written" (not the number of bytes in the file)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_ret_" TC_S1(TC_CRT_VER) ".tmp"
#define WFN L"" FN
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
TC_CASE(utf8_stream_U_4E2D_returns_1_writes_3_bytes)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%c", (int)0x4E2D); fclose(f);
    CHECK_EQ_I(r, 1);
    n = slurp(b, 64); CHECK_EQ_I(n, 3 + 3);
    DeleteFileA(FN);
}
TC_CASE(utf16_stream_returns_n)
{
    FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int r;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%ls%d", L"\x4E2D\x6587", 100); fclose(f);
    CHECK_EQ_I(r, 5);
    DeleteFileA(FN);
}
TC_CASE(file_size_vs_ret)
{
    unsigned char b[64]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-8"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%ls", L"\x00E9\x4E2D" L"z"); fclose(f);
    CHECK_EQ_I(r, 3);
    n = slurp(b, 64); CHECK_EQ_I(n, 3 + 2 + 3 + 1); CHECK(n - 3 != r);
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(utf8_stream_U_4E2D_returns_1_writes_3_bytes); TC_RUN(utf16_stream_returns_n); TC_RUN(file_size_vs_ret);
    return tc_end();
}
