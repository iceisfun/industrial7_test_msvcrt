/* TC
 * id:    loc_neg_free_locale_null
 * area:  locale
 * funcs: _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _free_locale
 * note:  _free_locale(NULL) must be a harmless no-op and must not call the invalid-parameter handler.
 */
#include "tc.h"
#include <locale.h>

static _locale_t volatile v_null;

TC_CASE(free_null_no_crash) { _free_locale(v_null); CHECK(1); }
TC_CASE(iph_not_invoked)
{
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    _free_locale(v_null);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(free_null_no_crash); TC_RUN(iph_not_invoked); return tc_end(); }
