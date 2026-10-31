/* TC
 * id:    loc_neg_mbtowc_incomplete_dbcs
 * area:  locale
 * funcs: mbtowc setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs mbtowc: -1 if the next count bytes do not form a valid multibyte character
 * note:  only the count-too-small lone lead byte case; errno not asserted. Invalid-trail cases are in loc_neg_mbtowc_bad_trail_probe. Skips if .932 is rejected.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static const char * volatile v_loc = ".932";
static int enter(void) { return setlocale(LC_ALL, (const char *)v_loc) != NULL; }
static char lead[2] = { (char)0x82, 0 };

TC_CASE(lone_lead_n1)
{
    wchar_t wc = 0x1234;
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mbtowc(&wc, lead, 1), -1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lone_lead_n1); return tc_end(); }
