/* TC
 * id:    pfs_neg_hs_invalid_multibyte
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field (%hs in wide functions); mbtowc
 * note:  PROBE: %hs of a lone DBCS lead byte (0x81) in code page 932 inside a wide printf. Expected -1 / EILSEQ; older CRTs may
 *        instead emit '?' or stop. The valid pair control (0x82 0xA0 = U+3042) is expected to work everywhere.
 *        Skipped if Japanese_Japan.932 is unavailable.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static const char * volatile lead_only = "\x81";
static const char * volatile pair = "\x82\xA0";

TC_CASE(valid_pair_control)
{
    wchar_t w[8]; int r;
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("no cp932 locale");
    r = _snwprintf(w, 8, L"%hs", pair);
    CHECK_EQ_I(r, 1);
    CHECK_EQ_I(w[0], 0x3042);
    setlocale(LC_ALL, "C");
}
TC_CASE(lead_without_trail)
{
    wchar_t w[8]; int r;
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("no cp932 locale");
    r = _snwprintf(w, 8, L"%hs", lead_only);
    CHECK_EQ_I(r, -1);
    setlocale(LC_ALL, "C");
}
TC_CASE(lead_without_trail_errno)
{
    wchar_t w[8];
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("no cp932 locale");
    errno = 0; (void)_snwprintf(w, 8, L"%hs", lead_only);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(valid_pair_control); TC_RUN(lead_without_trail); TC_RUN(lead_without_trail_errno); setlocale(LC_ALL, "C"); return tc_end(); }
