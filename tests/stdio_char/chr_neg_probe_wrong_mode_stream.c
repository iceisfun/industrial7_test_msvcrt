/* TC
 * id:    chr_neg_probe_wrong_mode_stream
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fgets fputc fputs _getw _putw fopen
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.5.3 (input only on read streams); MS docs silent on exact errno
 * note:  PROBE: reading from a write-only FILE and writing to a read-only FILE return EOF/NULL and set the error indicator. errno value is not asserted.
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

TC_CASE(read_from_write_only)
{
    FILE *f; char b[8];
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), EOF); CHECK(ferror(f) != 0); clearerr(f);
    CHECK(fgets(b, 8, f) == NULL); CHECK(ferror(f) != 0); clearerr(f);
    CHECK_EQ_I(_getw(f), EOF); CHECK(ferror(f) != 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(write_to_read_only)
{
    FILE *f; put_raw(FN, "abc", 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputc('x', f), EOF); CHECK(ferror(f) != 0); clearerr(f);
    CHECK_EQ_I(fputs("x", f), EOF); CHECK(ferror(f) != 0); clearerr(f);
    CHECK_EQ_I(_putw(7, f), EOF); CHECK(ferror(f) != 0);
    fclose(f);
    {   unsigned char raw[8]; DWORD n = get_raw(FN, raw, sizeof raw); CHECK_EQ_U(n, 3); }
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(read_from_write_only); TC_RUN(write_to_read_only);
    return tc_end();
}
