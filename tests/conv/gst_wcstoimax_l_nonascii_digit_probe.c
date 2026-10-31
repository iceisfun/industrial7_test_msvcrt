/* TC
 * id:    gst_wcstoimax_l_nonascii_digit_probe
 * area:  conv
 * funcs: _wcstoimax_l wcstoumax _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs wcstol (remarks); C11 7.29.4.1.2 leaves non-basic digits unspecified
 * note:  PROBES: the UCRT wide parser is believed to accept Unicode decimal digits (ARABIC-INDIC DIGIT THREE U+0663,
 *        FULLWIDTH DIGIT ONE U+FF11) as digit values 3 and 1 even in the "C" locale. Unspecified by ISO C. Pre-UCRT CRTs
 *        are deliberately not listed (behaviour unknown). Real Windows decides; a failure here is a finding, not a bug.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _wcstoimax_l(const wchar_t *, wchar_t **, int, _locale_t);

static const wchar_t *volatile w;
static _locale_t loc;

TC_CASE(arabic_indic_digit) { wchar_t *e = NULL; w = L"\x0663"; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), 3); CHECK_EQ_I(e - w, 1); }
TC_CASE(fullwidth_digit)    { wchar_t *e = NULL; w = L"\xff11"; CHECK_EQ_U(wcstoumax(w, &e, 10), 1); CHECK_EQ_I(e - w, 1); }
TC_CASE(run_with_nonascii)  { wchar_t *e = NULL; w = L"12\x0663" L"4"; CHECK_EQ_I(_wcstoimax_l(w, &e, 10, loc), 12 * 100 + 3 * 10 + 4); CHECK_EQ_I(e - w, 4); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(arabic_indic_digit); TC_RUN(fullwidth_digit); TC_RUN(run_with_nonascii);
    _free_locale(loc);
    return tc_end();
}
