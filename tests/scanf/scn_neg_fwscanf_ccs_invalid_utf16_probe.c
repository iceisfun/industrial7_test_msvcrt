/* TC
 * id:    scn_neg_fwscanf_ccs_invalid_utf16_probe
 * area:  scanf
 * kind:  negative
 * funcs: fwscanf fopen fclose
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs=UNICODE / UTF-16LE; BOM in existing file selects the encoding)
 * note:  PROBE: UTF-16LE file = BOM + 'A' + a single dangling byte (odd length, 3 payload bytes). The first %lc is probed to
 *        yield 'A' (return 1); the second read, which only has half a code unit, is probed to return EOF without
 *        producing a character.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wfile(const char *name, const void *data, unsigned len)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, data, len, &w, NULL); CloseHandle(h); }
}
#define FN "tc_scn_neg_odd16.tmp"
static const unsigned char D[] = { 0xFF, 0xFE, 'A', 0, 'B' };

TC_CASE(first_char)
{
    FILE *f; wchar_t c = 0; int n; wfile(FN, D, sizeof D); f = fopen(FN, "r,ccs=UTF-16LE");
    CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    n = fwscanf(f, L"%lc", &c); fclose(f); DeleteFileA(FN);
    CHECK_EQ_I(n, 1); CHECK_EQ_U(c, 'A');
}
TC_CASE(second_is_eof)
{
    FILE *f; wchar_t c = 0, d = 0x1234; int n; wfile(FN, D, sizeof D); f = fopen(FN, "r,ccs=UTF-16LE");
    CHECK(f != NULL); if (!f) { DeleteFileA(FN); return; }
    n = fwscanf(f, L"%lc", &c); CHECK_EQ_I(n, 1);
    n = fwscanf(f, L"%lc", &d); fclose(f); DeleteFileA(FN);
    CHECK_EQ_I(n, EOF); CHECK_EQ_U(d, 0x1234);
}
int main(void) { tc_begin(); TC_RUN(first_char); TC_RUN(second_is_eof); return tc_end(); }
