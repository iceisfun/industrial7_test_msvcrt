/* TC
 * id:    cv_neg_wcstod_ws_endptr_probe
 * area:  conv
 * funcs: wcstod
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3p8 (no conversion: endptr = nptr, i.e. BEFORE any skipped whitespace)
 * note:  PROBES: wcstod on a string with only leading whitespace / whitespace + non-number returns 0 and endptr == nptr (the ISO value).
 *        Pre-UCRT CRTs are suspected to set endptr past the skipped whitespace instead; real Windows decides.
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

TC_CASE(blank)       { none(L"   "); }
TC_CASE(ws_alpha)    { none(L"  abc"); }
TC_CASE(ws_sign_x)   { none(L" \t\n -x"); }
TC_CASE(ws_sign)     { none(L"   +"); }

int main(void)
{
    tc_begin();
    TC_RUN(blank); TC_RUN(ws_alpha); TC_RUN(ws_sign_x); TC_RUN(ws_sign);
    return tc_end();
}
