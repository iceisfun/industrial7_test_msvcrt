/* TC
 * id:    pfs_neg_probe_hc_lead_byte_wide
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   UCRT: %hc in wide printf converts a narrow char with the current locale's multibyte rules; failure -> EILSEQ, -1
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): in a cp932 locale a lone DBCS lead byte 0x81 passed to %hc yields return 0 (nothing written) with errno EILSEQ, not -1.
 *        Locale dependent: skips when the cp932 locale is missing. Control: ASCII 'A' and the single-byte katakana 0xA1 (U+FF61) convert. Cases skip when the Japanese locale is not installed.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

static const wchar_t *volatile wf_hc = L"%hc";
static volatile int lead = 0x81;
static volatile int kata = 0xA1;
static volatile int ascA = 'A';

TC_CASE(lead_byte_fails)
{
    wchar_t w[8]; int n;
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) { setlocale(LC_ALL, "C"); TC_SKIP("no cp932 locale"); }
    errno = 0;
    n = _snwprintf(w, 8, wf_hc, (char)lead);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, 0); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(katakana_single_byte_ok)
{
    wchar_t w[8]; int n;
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) { setlocale(LC_ALL, "C"); TC_SKIP("no cp932 locale"); }
    n = _snwprintf(w, 8, wf_hc, (char)kata);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, 1); CHECK(w[0] == 0xFF61);
}
TC_CASE(ascii_control)
{
    wchar_t w[8]; int n;
    if (!setlocale(LC_ALL, "Japanese_Japan.932")) { setlocale(LC_ALL, "C"); TC_SKIP("no cp932 locale"); }
    n = _snwprintf(w, 8, wf_hc, (char)ascA);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, 1); CHECK(w[0] == L'A');
}
int main(void)
{
    tc_begin();
    TC_RUN(lead_byte_fails); TC_RUN(katakana_single_byte_ok); TC_RUN(ascii_control);
    return tc_end();
}
