/* TC
 * id:    loc_neg_probe_create_locale_category_boundaries
 * area:  locale
 * funcs: _create_locale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale (category must be LC_ALL..LC_TIME)
 * note:  PROBE: _create_locale with category INT_MAX / INT_MIN returns NULL; global state unchanged. errno is NOT asserted here
 *        (see loc_neg_probe_create_locale_category_errno).
 */
#include "tc.h"
#include <locale.h>
#include <limits.h>
#include <errno.h>

static volatile int v_cat;

TC_CASE(int_max_rejected)
{
    tc_iph_install(); v_cat = INT_MAX;
    CHECK(_create_locale(v_cat, "C") == NULL);
}
TC_CASE(int_min_rejected)
{
    tc_iph_install(); v_cat = INT_MIN;
    CHECK(_create_locale(v_cat, "C") == NULL);
}
TC_CASE(global_unchanged)
{
    setlocale(LC_ALL, "C"); tc_iph_install(); v_cat = INT_MAX;
    _create_locale(v_cat, "C");
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(int_max_rejected); TC_RUN(int_min_rejected); TC_RUN(global_unchanged); return tc_end(); }
