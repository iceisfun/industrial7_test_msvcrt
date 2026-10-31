/* TC
 * id:    loc_neg_create_locale_bad_category
 * area:  locale
 * funcs: _create_locale _free_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale (category must be a LC_* value); parameter validation (EINVAL + invalid parameter handler)
 * note:  PROBE: out-of-range category returns NULL, sets errno to EINVAL and invokes the invalid-parameter handler.
 *        Result checks are separate from errno / handler checks.
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
    CHECK_EQ_I(errno, EINVAL); if (l) _free_locale(l);
}
TC_CASE(iph_invoked)
{
    _locale_t l; int before;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    before = tc_iph_count; l = _create_locale(cat_neg, n_c);
    CHECK_EQ_I(tc_iph_count - before, 1); if (l) _free_locale(l);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(category_minus1); TC_RUN(category_max_plus1); TC_RUN(errno_einval); TC_RUN(iph_invoked);
    return tc_end();
}
