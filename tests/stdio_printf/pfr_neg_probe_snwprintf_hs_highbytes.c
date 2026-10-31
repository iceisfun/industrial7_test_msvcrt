/* TC
 * id:    pfr_neg_probe_snwprintf_hs_highbytes
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf %hs in wide functions; mbtowc
 * note:  PROBE: %hs with bytes > 0x7F in the "C" locale. Only asserts no crash and errno is 0 or EILSEQ, and that the
 *        return value is -1 or 3 (one wchar per byte, the likely C-locale 1:1 mapping).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(hs_high)
{
    wchar_t b[8]; const wchar_t * volatile f = L"%hs"; const char * volatile s = "\xE4\xB8\xAD"; int r;
    errno = 0; r = _snwprintf(b, 8, f, s);
    CHECK(r == 3 || r == -1);
    CHECK(errno == 0 || errno == EILSEQ);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(hs_high); return tc_end(); }
