/* TC
 * id:    loc_neg_mbtowc_count_zero
 * area:  locale
 * funcs: mbtowc _mbtowc_l mblen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.7.1 (MS believed to deviate, see note)
 * note:  PROBE: n==0 is not an error in the MS implementation: mbtowc(&wc,"a",0) and mblen("a",0) return 0 (treated like the
 *        empty/state-query case), not -1, and wc is untouched. Differs from C11 wording; believed from the ucrt sources.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static volatile size_t v_n = 0;

TC_CASE(count0_returns_0)
{
    wchar_t wc = 0x1234; char s[2] = { 'a', 0 };
    CHECK_EQ_I(mbtowc(&wc, s, v_n), 0);
    CHECK_EQ_I(wc, 0x1234);
    CHECK_EQ_I(_mbtowc_l(&wc, s, v_n, NULL), 0);
    CHECK_EQ_I(mblen(s, v_n), 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(count0_returns_0); return tc_end(); }
