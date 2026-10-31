/* TC
 * id:    pfl_probe_percent_c_wide_conv_locale
 * area:  stdio_printf
 * kind:  positive
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field (%lc, %ls convert to multibyte per the locale); wctomb / EILSEQ
 * note:  PROBE: narrow printf converting wide arguments (%lc / %ls) uses the passed LOCALE's code page.
 *        Expected: German 1252: U+00E4 -> 0xE4, U+20AC -> 0x80; U+0100 not representable -> return -1 and errno EILSEQ;
 *        "C" locale: code points <= 0xFF pass through as the same byte (UCRT/MSVCRT C-locale behaviour), > 0xFF -> -1/EILSEQ.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#include <locale.h>

TC_CASE(lc_e4_to_cp1252)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[16]; int r; volatile wint_t c = 0xE4;
    if (!l) TC_SKIP("German locale not installed");
    r = _sprintf_l(b, "%lc", l, c);
    CHECK_EQ_I(r, 1); CHECK_EQ_I((unsigned char)b[0], 0xE4);
    _free_locale(l);
}
TC_CASE(ls_euro_cp1252)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[16]; int r;
    volatile const wchar_t *w = L"\x20AC";
    if (!l) TC_SKIP("German locale not installed");
    r = _sprintf_l(b, "%ls", l, (const wchar_t *)w);
    CHECK_EQ_I(r, 1); CHECK_EQ_I((unsigned char)b[0], 0x80);
    _free_locale(l);
}
TC_CASE(ls_unconvertible_char_ret_and_errno_eilseq)
{
    _locale_t l = _create_locale(LC_ALL, "German_Germany.1252"); char b[16]; int r;
    volatile const wchar_t *w = L"a\x0100z";
    if (!l) TC_SKIP("German locale not installed");
    errno = 0;
    r = _sprintf_l(b, "%ls", l, (const wchar_t *)w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
    _free_locale(l);
}
TC_CASE(sprintf_l_c_locale_u00e4_ret)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; int r; volatile wint_t c = 0xE4;
    CHECK(l != NULL); if (!l) return;
    r = _sprintf_l(b, "%lc", l, c);
    CHECK_EQ_I(r, 1); CHECK_EQ_I((unsigned char)b[0], 0xE4);
    _free_locale(l);
}
TC_CASE(c_locale_u0100_eilseq)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; int r; volatile wint_t c = 0x100;
    CHECK(l != NULL); if (!l) return;
    errno = 0;
    r = _sprintf_l(b, "%lc", l, c);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(lc_e4_to_cp1252); TC_RUN(ls_euro_cp1252); TC_RUN(ls_unconvertible_char_ret_and_errno_eilseq);
    TC_RUN(sprintf_l_c_locale_u00e4_ret); TC_RUN(c_locale_u0100_eilseq);
    return tc_end();
}
