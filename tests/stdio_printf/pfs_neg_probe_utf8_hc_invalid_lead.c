/* TC
 * id:    pfs_neg_probe_utf8_hc_invalid_lead
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   UCRT supports a ".UTF-8" locale (Windows 10 1803+); invalid multibyte input to %hc/%hs in wide printf -> EILSEQ, -1
 * note:  PROBE: in the UTF-8 locale a lone lead byte 0xC3 via %hc, and truncated "\xC3", invalid "\xFF", overlong "\xC0\x80" via %hs all fail. Skipped when setlocale(".UTF-8") fails. Overlong rejection is the least certain.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

static const wchar_t *volatile wf_hc = L"%hc";
static const wchar_t *volatile wf_hs = L"%hs";
static volatile int c3 = 0xC3;
static const char *volatile s_trunc = "\xC3";
static const char *volatile s_ff = "\xFF";
static const char *volatile s_overlong = "\xC0\x80";

#define NEED_UTF8() do { if (!setlocale(LC_ALL, ".UTF-8")) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8 locale"); } } while (0)

TC_CASE(hc_lone_lead)
{
    wchar_t w[8]; int n; NEED_UTF8(); errno = 0;
    n = _snwprintf(w, 8, wf_hc, (char)c3);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(hs_truncated)
{
    wchar_t w[8]; int n; NEED_UTF8(); errno = 0;
    n = _snwprintf(w, 8, wf_hs, s_trunc);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(hs_ff)
{
    wchar_t w[8]; int n; NEED_UTF8(); errno = 0;
    n = _snwprintf(w, 8, wf_hs, s_ff);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(hs_overlong)
{
    wchar_t w[8]; int n; NEED_UTF8();
    n = _snwprintf(w, 8, wf_hs, s_overlong);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(n, -1);
}
int main(void)
{
    tc_begin();
    TC_RUN(hc_lone_lead); TC_RUN(hs_truncated); TC_RUN(hs_ff); TC_RUN(hs_overlong);
    return tc_end();
}
