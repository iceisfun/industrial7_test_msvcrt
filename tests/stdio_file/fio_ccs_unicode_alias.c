/* TC
 * id:    fio_ccs_unicode_alias
 * area:  stdio_file
 * funcs: fopen fclose fputwc
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fopen (ccs=UNICODE is UTF-16LE)
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_ccs_unicode_alias_" TC_VS ".tmp";
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
static int write_with(const char *mode, unsigned char *b)
{
    FILE *f = fopen(NAME, mode); int n;
    if (!f) return -2;
    fputwc((wchar_t)0x4E2D, f); fputwc(L'Z', f);
    fclose(f);
    n = rawread(NAME, b, 16); DeleteFileA(NAME); return n;
}
static const unsigned char want[] = { 0xFF, 0xFE, 0x2D, 0x4E, 0x5A, 0x00 };
TC_CASE(same_as_utf16le)
{
    unsigned char b[16], c[16];
    CHECK_EQ_I(write_with("w, ccs=UNICODE", b), 6);
    CHECK_EQ_I(write_with("w, ccs=UTF-16LE", c), 6);
    CHECK_MEM(b, c, 6); CHECK_MEM(b, want, 6);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(same_as_utf16le); return tc_end(); }
