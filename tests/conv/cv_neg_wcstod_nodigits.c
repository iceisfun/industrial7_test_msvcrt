/* TC
 * id:    cv_neg_wcstod_nodigits
 * area:  conv
 * funcs: wcstod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3p8 (no conversion: zero returned, endptr = nptr); MS docs wcstod
 * note:  Inputs with LEADING WHITESPACE are not here: legacy (pre-UCRT) CRTs are believed to report endptr after the skipped whitespace
 *        on no-conversion; see cv_neg_wcstod_ws_endptr_probe. errno behaviour on no-conversion is not asserted here (see cv_neg_wcsto_nodigits_errno_probe).
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

static const wchar_t *volatile v_s;

static void none(const wchar_t *s)
{
    wchar_t *e = (wchar_t *)1; double r;
    v_s = s; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, 0.0);
    CHECK_EQ_P(e, s);
}

TC_CASE(empty_blank_alpha) { none(L""); none(L"abc"); }
TC_CASE(sign_only)         { none(L"+"); none(L"-"); none(L"-."); none(L"+."); }
TC_CASE(point_exp_only)    { none(L".e5"); none(L"e5"); none(L"E+5"); }
TC_CASE(space_after_sign)  { none(L"- 1"); none(L"+ 1"); }

int main(void)
{
    tc_begin();
    TC_RUN(empty_blank_alpha); TC_RUN(sign_only); TC_RUN(point_exp_only); TC_RUN(space_after_sign);
    return tc_end();
}
