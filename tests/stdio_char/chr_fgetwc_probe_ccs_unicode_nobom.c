/* TC
 * id:    chr_fgetwc_probe_ccs_unicode_nobom
 * area:  stdio_char
 * funcs: fgetwc fopen
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen ccs=UNICODE
 * note:  PROBE: a file without BOM opened for reading with ccs=UNICODE is read as UTF-16LE (documented default as remembered). Probes exactly that.
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

TC_CASE(nobom_defaults_utf16le)
{
    FILE *f; wint_t c; static const unsigned char d[] = { 'h', 0, 0x3A, 0x26 };
    put_raw(FN, d, sizeof d);
    f = fopen(FN, "rt, ccs=UNICODE"); CHECK(f != NULL); if (!f) return;
    c = fgetwc(f); CHECK_EQ_U(c, 'h');
    c = fgetwc(f); CHECK_EQ_U(c, 0x263A);
    c = fgetwc(f); CHECK_EQ_U(c, WEOF);
    fclose(f); DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(nobom_defaults_utf16le); return tc_end(); }
