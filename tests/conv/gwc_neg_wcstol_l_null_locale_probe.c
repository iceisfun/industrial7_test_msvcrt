/* TC
 * id:    gwc_neg_wcstol_l_null_locale_probe
 * area:  conv
 * funcs: _wcstol_l _wcstod_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strtol_l/_wcstol_l (NULL locale = current locale is the usual convention for _l functions)
 * note:  PROBES: a NULL _locale_t is accepted (falls back to the current locale) and does NOT call the invalid-parameter handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t *volatile vp;

TC_CASE(wcstol_null_locale)
{
    wchar_t *e = NULL; long r; tc_iph_count = 0; errno = 0;
    vp = L"42x"; r = _wcstol_l(vp, &e, 10, NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(r, 42);
    CHECK(e == vp + 2);
}
TC_CASE(wcstod_null_locale)
{
    wchar_t *e = NULL; double r; tc_iph_count = 0; errno = 0;
    vp = L"1.5"; r = _wcstod_l(vp, &e, NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_BITS_D(r, 1.5);
    CHECK(e == vp + 3);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(wcstol_null_locale); TC_RUN(wcstod_null_locale);
    return tc_end();
}
