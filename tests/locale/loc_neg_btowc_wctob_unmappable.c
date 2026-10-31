/* TC
 * id:    loc_neg_btowc_wctob_unmappable
 * area:  locale
 * funcs: btowc wctob setlocale
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.1.1 btowc (WEOF if not a valid single-byte char), 7.29.6.1.2 wctob (EOF if not a single byte)
 * note:  Requires code pages 1252 and 932 to be available (skipped otherwise).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

TC_CASE(wctob_unmappable_eof)
{
    volatile const char *n = ".1252"; volatile wint_t w = 0x0416;
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("1252 rejected"); }
    CHECK_EQ_I(wctob(w), EOF);
    setlocale(LC_ALL, "C");
}
TC_CASE(btowc_lead_byte_weof)
{
    volatile const char *n = ".932"; volatile int c = 0x82;
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("932 rejected"); }
    CHECK_EQ_U(btowc(c), WEOF);
    setlocale(LC_ALL, "C");
}
TC_CASE(wctob_dbcs_char_eof)
{
    volatile const char *n = ".932"; volatile wint_t w = 0x3042;
    if (!setlocale(LC_ALL, (const char *)n)) { setlocale(LC_ALL, "C"); TC_SKIP("932 rejected"); }
    CHECK_EQ_I(wctob(w), EOF);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(wctob_unmappable_eof); TC_RUN(btowc_lead_byte_weof); TC_RUN(wctob_dbcs_char_eof); return tc_end(); }
