/* TC
 * id:    fio_ccs_utf16le_read_decode
 * area:  stdio_file
 * funcs: fopen fclose fgetwc fwrite
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (ccs=UTF-16LE), fgetwc
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_ccs_utf16le_read_decode_" TC_VS ".tmp";
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
TC_CASE(with_bom)
{
    static const unsigned char d[] = { 0xFF, 0xFE, 0x41, 0x00, 0x2D, 0x4E };
    FILE *f; rawwrite(NAME, d, sizeof d);
    f = fopen(NAME, "r, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    CHECK_EQ_I(fgetwc(f), 0x41);
    CHECK_EQ_I(fgetwc(f), 0x4E2D);
    CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); DeleteFileA(NAME);
}
TC_CASE(no_bom)
{
    static const unsigned char d[] = { 0x41, 0x00, 0x2D, 0x4E };
    FILE *f; rawwrite(NAME, d, sizeof d);
    f = fopen(NAME, "r, ccs=UTF-16LE"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    CHECK_EQ_I(fgetwc(f), 0x41);
    CHECK_EQ_I(fgetwc(f), 0x4E2D);
    CHECK_EQ_U(fgetwc(f), WEOF);
    CHECK(feof(f) != 0);
    fclose(f); DeleteFileA(NAME);
}
TC_CASE(written_by_fwrite)
{
    FILE *f = fopen(NAME, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("\xFF\xFE\x5A\x00", 1, 4, f), 4); fclose(f);
    f = fopen(NAME, "r, ccs=UNICODE"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    CHECK_EQ_I(fgetwc(f), 0x5A); CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(with_bom); TC_RUN(no_bom); TC_RUN(written_by_fwrite); return tc_end(); }
