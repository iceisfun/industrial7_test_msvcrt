/* TC
 * id:    pfv_neg_probe_narrow_to_wide_eilseq
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf _snwprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs setlocale (".65001" UTF-8 locale, Windows 10 1803+); printf %hs in wide functions converts through mbrtowc
 * note:  PROBE: with the UTF-8 locale active, "%hs" of the invalid UTF-8 bytes FF FE in a wide printf fails: -1 with errno
 *        EILSEQ. The valid-UTF-8 control must succeed. Cases SKIP when setlocale(".65001") is unavailable. The locale is
 *        always restored to "C".
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static const char *volatile bad = "\xff\xfe";
static const char *volatile good = "ab";
static const wchar_t *volatile f_hs = L"%hs";

TC_CASE(hs_invalid_utf8)
{
    wchar_t b[8]; int r;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("no utf-8 locale");
    errno = 0; r = swprintf(b, 8, f_hs, bad);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, -1);
}
TC_CASE(errno_eilseq)
{
    wchar_t b[8]; int r, e;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("no utf-8 locale");
    errno = 0; r = swprintf(b, 8, f_hs, bad); e = errno;
    setlocale(LC_ALL, "C");
    (void)r; CHECK_EQ_I(e, EILSEQ);
}
TC_CASE(snwprintf_invalid_utf8)
{
    wchar_t b[8]; int r;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("no utf-8 locale");
    r = _snwprintf(b, 8, f_hs, bad);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, -1);
}
TC_CASE(valid_utf8_control)
{
    wchar_t b[8]; int r;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("no utf-8 locale");
    r = swprintf(b, 8, f_hs, good);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, 2); CHECK_WSTR(b, L"ab");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(hs_invalid_utf8); TC_RUN(errno_eilseq); TC_RUN(snwprintf_invalid_utf8); TC_RUN(valid_utf8_control);
    setlocale(LC_ALL, "C");
    return tc_end();
}
