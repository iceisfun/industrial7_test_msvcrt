/* TC
 * id:    chr_fgetwc_probe_ccs_unicode_nobom
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen ccs=UNICODE
 * note:  PROBE: a file without BOM opened for reading with ccs=UNICODE is read as UTF-16LE per the MS docs fopen BOM table (UNICODE/no BOM -> UTF-16LE). Probes exactly that.
 *        OBSERVED Win10 22H2 CONTRADICTS the MS docs fopen BOM table (UNICODE/no BOM -> UTF-16LE); expectation follows Windows.
 *        OBSERVED (Win10 22H2 x64 reference, class A): no-BOM "rt, ccs=UNICODE" input is NOT decoded as UTF-16LE; bytes 68 00 3A 26 read back as
 *        L'h', 0x00, 0x3A, 0x26, WEOF (raw: c == 0x263A got=0x0 / c == WEOF got=0x3a). Stable across runs; the BOM, when present, decides the encoding.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

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

TC_CASE(nobom_reads_bytes)
{
    FILE *f; wint_t c; static const unsigned char d[] = { 'h', 0, 0x3A, 0x26 };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UNICODE"); CHECK(f != NULL); if (!f) return;
    /* observed Win10 22H2 ucrtbase: with no BOM the file is read as single-byte text, one wide char per byte */
    c = fgetwc(f); CHECK_EQ_U(c, 'h');
    c = fgetwc(f); CHECK_EQ_U(c, 0x00);
    c = fgetwc(f); CHECK_EQ_U(c, 0x3A);
    c = fgetwc(f); CHECK_EQ_U(c, 0x26);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(nobom_reads_bytes); return tc_end(); }
