/* TC
 * id:    gst_wcstoimax_l_basic
 * area:  conv
 * funcs: _wcstoimax_l _wcstoumax_l wcstoumax _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs wcstoimax, _wcstoimax_l, wcstoumax, _wcstoumax_l; C11 7.8.2.4
 * note:  wcstoimax (non-_l) is covered by ucr_strtoimax_wcstoimax. Non-ASCII digit handling is in gst_wcstoimax_l_neg_nonascii_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _wcstoimax_l(const wchar_t *, wchar_t **, int, _locale_t);
extern uintmax_t __cdecl _wcstoumax_l(const wchar_t *, wchar_t **, int, _locale_t);

static const wchar_t *volatile w;
static _locale_t loc;

TC_CASE(forms)
{
    wchar_t *e;
    w = L"  -1234567890123x"; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), -1234567890123LL); CHECK_EQ_P(e, w + 16);
    w = L"0xAbC"; CHECK_EQ_I(_wcstoimax_l(w, &e, 16, loc), 0xabc); CHECK_EQ_P(e, w + 5);
    w = L"0755";  CHECK_EQ_I(_wcstoimax_l(w, &e, 8, loc), 493);
    w = L"0755";  CHECK_EQ_I(_wcstoimax_l(w, &e, 0, loc), 493);
    w = L"0x1f";  CHECK_EQ_I(_wcstoimax_l(w, &e, 0, loc), 31);
    w = L"123";   CHECK_EQ_I(_wcstoimax_l(w, &e, 0, loc), 123);
    w = L"zz";    CHECK_EQ_U(_wcstoumax_l(w, &e, 36, loc), 1295);
    w = L"101";   CHECK_EQ_U(wcstoumax(w, &e, 2), 5);
}
TC_CASE(limits)
{
    wchar_t *e;
    w = L"9223372036854775807";  CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), INTMAX_MAX); CHECK_EQ_P(e, w + 19);
    w = L"-9223372036854775808"; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), INTMAX_MIN); CHECK_EQ_P(e, w + 20);
    w = L"18446744073709551615"; CHECK_EQ_U(_wcstoumax_l(w, &e, 10, loc), UINTMAX_MAX); CHECK_EQ_P(e, w + 20);
    CHECK_EQ_U(wcstoumax(w, &e, 10), UINTMAX_MAX);
}
TC_CASE(wcstoumax_plus42)
{
    wchar_t *e1, *e2;
    w = L"  +42"; CHECK_EQ_U(wcstoumax(w, &e1, 10), 42); CHECK_EQ_P(e1, w + 5);
    CHECK_EQ_U(_wcstoumax_l(w, &e2, 10, loc), 42); CHECK_EQ_P(e2, e1);
}
TC_CASE(endptr_wchar_units)
{
    wchar_t *e;
    w = L"77 end"; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), 77); CHECK_EQ_I(e - w, 2);
    w = L"12\x00e9"; CHECK_EQ_U(wcstoumax(w, &e, 10), 12); CHECK_EQ_I(e - w, 2);
}
TC_CASE(negative_unsigned_wraps)
{
    wchar_t *e;
    w = L"-1"; errno = 0; CHECK_EQ_U(wcstoumax(w, &e, 10), UINTMAX_MAX); CHECK_EQ_I(errno, 0);
    CHECK_EQ_U(_wcstoumax_l(w, &e, 10, loc), UINTMAX_MAX);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(forms); TC_RUN(limits); TC_RUN(wcstoumax_plus42); TC_RUN(endptr_wchar_units); TC_RUN(negative_unsigned_wraps);
    _free_locale(loc);
    return tc_end();
}
