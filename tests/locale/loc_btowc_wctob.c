/* TC
 * id:    loc_btowc_wctob
 * area:  locale
 * funcs: btowc wctob setlocale
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.6.1.1 btowc, 7.29.6.1.2 wctob; MS docs btowc, wctob
 * note:  btowc/wctob are exported by msvcr110+ only in the mingw import libs (msvcr100 lacks them).
 *        The 1252 case is documented code-page mapping (0x80 = EURO SIGN).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

TC_CASE(btowc_ascii) { volatile int c = 'a'; CHECK_EQ_U(btowc(c), L'a'); c = 'Z'; CHECK_EQ_U(btowc(c), L'Z'); c = 0; CHECK_EQ_U(btowc(c), 0); }
TC_CASE(btowc_eof)   { volatile int c = EOF; CHECK_EQ_U(btowc(c), WEOF); }
TC_CASE(wctob_ascii) { volatile wint_t w = L'a'; CHECK_EQ_I(wctob(w), 'a'); w = 0; CHECK_EQ_I(wctob(w), 0); }
TC_CASE(wctob_weof)  { volatile wint_t w = WEOF; CHECK_EQ_I(wctob(w), EOF); }
TC_CASE(cp1252_euro_both_ways)
{
    volatile const char *n = ".1252"; volatile int c = 0x80; volatile wint_t w = 0x20AC;
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("1252 rejected"); }
    CHECK_EQ_U(btowc(c), 0x20AC);
    CHECK_EQ_I(wctob(w), 0x80);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(btowc_ascii); TC_RUN(btowc_eof); TC_RUN(wctob_ascii); TC_RUN(wctob_weof); TC_RUN(cp1252_euro_both_ways); return tc_end(); }
