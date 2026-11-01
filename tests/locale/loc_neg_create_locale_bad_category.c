/* TC
 * id:    loc_neg_create_locale_bad_category
 * area:  locale
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _create_locale (category must be a LC_* value); parameter validation (EINVAL + invalid parameter handler)
 * note:  Win10 triage (class A): observed on Windows 10 22H2 build 19045, _create_locale with an out-of-range category returns NULL silently: errno stays 0 and the invalid-parameter
 *        handler is NOT invoked (no parameter validation on this path). Assertions record that; stable across 2 runs.
 * note:  Original PROBE: out-of-range category returns NULL, sets errno to EINVAL and invokes the invalid-parameter handler.
 *        Result checks are separate from errno / handler checks.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <errno.h>
#include <locale.h>

static volatile int cat_neg = -1, cat_big = LC_MAX + 1;
static const char *volatile n_c = "C";

TC_CASE(category_minus1) { _locale_t l = _create_locale(cat_neg, n_c); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(category_max_plus1) { _locale_t l = _create_locale(cat_big, n_c); CHECK(l == NULL); if (l) _free_locale(l); }
TC_CASE(errno_einval)
{
    _locale_t l; errno = 0; l = _create_locale(cat_neg, n_c);
    CHECK_EQ_I(errno, 0); if (l) _free_locale(l);   /* observed: no errno */
}
TC_CASE(iph_invoked)
{
    _locale_t l; int before;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    before = tc_iph_count; l = _create_locale(cat_neg, n_c);
    CHECK_EQ_I(tc_iph_count - before, 0); if (l) _free_locale(l);   /* observed: handler not invoked */
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(category_minus1); TC_RUN(category_max_plus1); TC_RUN(errno_einval); TC_RUN(iph_invoked);
    return tc_end();
}
