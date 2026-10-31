/* TC
 * id:    loc_neg_probe_create_locale_category_errno
 * area:  locale
 * funcs: _create_locale
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale (no errno documented for a bad category)
 * note:  PROBE: an out-of-range category is believed to make _create_locale return NULL through a plain range check, WITHOUT
 *        setting errno and WITHOUT invoking the invalid-parameter handler (errno left at its prior value, handler count 0).
 *        If real Windows reports EINVAL / invokes the handler instead, this test documents that.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <limits.h>

static volatile int v_cat;

TC_CASE(errno_untouched)
{
    tc_iph_install(); tc_iph_count = 0; v_cat = INT_MAX; errno = 0;
    CHECK(_create_locale(v_cat, "C") == NULL);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(errno_untouched); return tc_end(); }
