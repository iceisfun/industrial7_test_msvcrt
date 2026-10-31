/* TC
 * id:    loc_neg_mblen_invalid
 * area:  locale
 * funcs: mblen setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mblen: -1 if the next count bytes do not form a valid multibyte char; C11 7.22.7.1
 * note:  Lone lead byte with count 1 only (bad trail byte is in loc_neg_mbtowc_bad_trail_probe). Skips if .932 is rejected. count==0 behaviour is in loc_neg_mbtowc_count_zero (probe).
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static char lead[2] = { (char)0x82, 0 };

TC_CASE(lone_lead_minus1)
{
    if (!setlocale(LC_ALL, ".932")) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mblen(lead, 1), -1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lone_lead_minus1); return tc_end(); }
