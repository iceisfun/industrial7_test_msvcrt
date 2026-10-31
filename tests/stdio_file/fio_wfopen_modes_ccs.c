/* TC
 * id:    fio_wfopen_modes_ccs
 * area:  stdio_file
 * funcs: _wfopen fclose fread fwrite
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen, _wfopen (ccs= encoding, BOM written for UTF-8 / UNICODE on write)
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_wfopen_modes_ccs_" TC_VS ".tmp";
static const wchar_t WNAME[] = L"tc_fio_wfopen_modes_ccs_" TC_WID(TC_VS) L".tmp";
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
TC_CASE(utf8_bom)
{
    unsigned char b[16] = {0}; FILE *f = _wfopen(WNAME, L"w, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 3);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    DeleteFileA(NAME);
}
TC_CASE(unicode_bom)
{
    unsigned char b[16] = {0}; FILE *f = _wfopen(WNAME, L"w, ccs=UNICODE");
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rawread(NAME, b, 16), 2);
    CHECK_EQ_U(b[0], 0xFF); CHECK_EQ_U(b[1], 0xFE);
    DeleteFileA(NAME);
}
TC_CASE(rb_readback)
{
    unsigned char b[16] = {0}; FILE *f = _wfopen(WNAME, L"wb"); size_t n;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("xyz", 1, 3, f), 3);
    CHECK_EQ_I(fclose(f), 0);
    f = _wfopen(WNAME, L"rb"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    n = fread(b, 1, 16, f); CHECK_EQ_U(n, 3); CHECK_MEM(b, "xyz", 3);
    fclose(f); DeleteFileA(NAME);
}
TC_CASE(missing_file)
{
    FILE *f = _wfopen(L"tc_fio_wfopen_modes_ccs_missing.tmp", L"rb");
    CHECK(f == NULL);
    if (f) fclose(f);
}
int main(void)
{
    tc_begin(); DeleteFileA(NAME);
    TC_RUN(utf8_bom); TC_RUN(unicode_bom); TC_RUN(rb_readback); TC_RUN(missing_file);
    return tc_end();
}
