/* TC
 * id:    pfx_probe_fwprintf_ccs_utf8_surrogate
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (ccs=UTF-8); Unicode UTF-8 encoding
 * note:  PROBE: a UTF-16 surrogate pair (U+1F600) written by fwprintf to a "w, ccs=UTF-8" stream is combined into the
 *        single 4-byte UTF-8 sequence F0 9F 98 80 (after the EF BB BF BOM) rather than two 3-byte CESU-style sequences.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile path = "tc_pfx_probe_fwprintf_surr_a.tmp";

TC_CASE(surrogate)
{
    unsigned char b[32] = {0}; DWORD n = 0; volatile int r;
    HANDLE h; FILE *f = fopen(path, "w, ccs=UTF-8");
    CHECK(f != NULL);
    if (!f) return;
    r = fwprintf(f, L"\xD83D\xDE00");
    CHECK_EQ_I(r, 2);
    fclose(f);
    h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) { ReadFile(h, b, sizeof b, &n, NULL); CloseHandle(h); }
    CHECK_EQ_U(n, 7);
    if (n == 7) {
        static const unsigned char want[] = { 0xEF, 0xBB, 0xBF, 0xF0, 0x9F, 0x98, 0x80 };
        CHECK_MEM(b, want, 7);
    }
    DeleteFileA(path);
}
int main(void) { tc_begin(); TC_RUN(surrogate); return tc_end(); }
