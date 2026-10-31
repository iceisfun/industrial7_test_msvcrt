/* TC
 * id:    mb_probe_setmbcp_locale
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _setmbcp (_MB_CP_LOCALE)
 * note:  probe: _setmbcp(_MB_CP_LOCALE) in the C locale; assert only ret==0 and getmbcp in {0, GetACP()}. _MB_CP_LOCALE is -4 (not -5, which is _MB_CP_UTF8 in newer UCRT).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

#define MBCP_LOCALE _MB_CP_LOCALE  /* -4 */
static volatile int v_cp = MBCP_LOCALE;
TC_CASE(locale)
{
    int s = _getmbcp(), r, g;
    r = _setmbcp(v_cp);
    g = _getmbcp();
    CHECK_EQ_I(r, 0);
    CHECK(g == 0 || g == (int)GetACP());
    _setmbcp(s);
}

int main(void) { tc_begin(); TC_RUN(locale); return tc_end(); }
