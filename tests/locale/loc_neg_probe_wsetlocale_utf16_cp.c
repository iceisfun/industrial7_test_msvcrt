/* TC
 * id:    loc_neg_probe_wsetlocale_utf16_cp
 * area:  locale
 * funcs: _wsetlocale
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (code page restrictions)
 * note:  PROBE: _wsetlocale(LC_ALL, L".1200") (UTF-16LE as locale code page) returns NULL and leaves the locale "C".
 *        Believed unsupported (multi-byte-per-unit encodings are not valid locale code pages) but not verified on every CRT.
 */
#include "tc.h"
#include <locale.h>

TC_CASE(utf16_cp_null)
{
    volatile const wchar_t *n = L".1200";
    wchar_t *r = _wsetlocale(LC_ALL, (const wchar_t *)n);
    CHECK(r == NULL);
    if (r) _wsetlocale(LC_ALL, L"C");
    CHECK_WSTR(_wsetlocale(LC_ALL, NULL), L"C");
}
int main(void) { tc_begin(); TC_RUN(utf16_cp_null); return tc_end(); }
