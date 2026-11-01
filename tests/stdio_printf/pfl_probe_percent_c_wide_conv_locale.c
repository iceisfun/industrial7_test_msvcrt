/* TC
 * id:    pfl_probe_percent_c_wide_conv_locale
 * area:  stdio_printf
 * kind:  positive
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field (%lc, %ls convert to multibyte per the locale); wctomb / EILSEQ
 * note:  Narrow printf converting wide arguments (%lc / %ls) uses the passed LOCALE's code page.
 *        Expected: German 1252: U+00E4 -> 0xE4, U+20AC -> 0x80; "C" locale: code points <= 0xFF pass through as the same byte (UCRT/MSVCRT C-locale behaviour).
 *        Observed Win10 22H2 ucrtbase (corrects the old guess of -1/EILSEQ): %ls with an unmappable char (U+0100) in German_1252 does not fail: best-fit maps it to 'A', returns 3 ("aAz"), errno 0; the same %ls in the "C" locale returns -1 with EILSEQ;
 *        %lc U+0100 in the "C" locale returns 0 (nothing written) with errno EILSEQ.
 *        Raw evidence (baseline): ls_unconvertible... r == -1 got=3 want=-1 ; errno == EILSEQ got=0 ; c_locale_u0100_eilseq r == -1 got=0 want=-1 (errno EILSEQ held).
 *        Locale-dependent: German_Germany.1252 must be installed (else the case skips).
 *        crt narrowed to ucrtbase: the observed behaviour was measured on Win10 22H2 ucrtbase only (older CRTs [unverified]).
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
    CHECK_EQ_I(r, 3); CHECK_EQ_I(errno, 0); CHECK_STR(b, "aAz");   /* observed: cp1252 best-fit maps U+0100 to 'A'; no failure */
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
TC_CASE(c_locale_ls_u0100_eilseq)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; int r; volatile const wchar_t *w = L"a\x0100z";
    CHECK(l != NULL); if (!l) return;
    errno = 0;
    r = _sprintf_l(b, "%ls", l, (const wchar_t *)w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);   /* observed: "C" locale has no best-fit: -1 / EILSEQ (output stops before the char) */
    _free_locale(l);
}
TC_CASE(c_locale_u0100_eilseq)
{
    _locale_t l = _create_locale(LC_ALL, "C"); char b[16]; int r; volatile wint_t c = 0x100;
    CHECK(l != NULL); if (!l) return;
    errno = 0;
    r = _sprintf_l(b, "%lc", l, c);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EILSEQ);   /* observed: nothing written, return 0 (not -1), errno EILSEQ */
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(lc_e4_to_cp1252); TC_RUN(ls_euro_cp1252); TC_RUN(ls_unconvertible_char_ret_and_errno_eilseq);
    TC_RUN(sprintf_l_c_locale_u00e4_ret); TC_RUN(c_locale_ls_u0100_eilseq); TC_RUN(c_locale_u0100_eilseq);
    return tc_end();
}
