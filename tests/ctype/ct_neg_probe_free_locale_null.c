/* TC
 * id:    ct_neg_probe_free_locale_null
 * area:  ctype
 * funcs: isalpha
 * procs: _free_locale
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _free_locale (NULL handling not stated)
 * note:  PROBE: _free_locale(NULL) is a harmless no-op (no crash, no IPH call).
 */
#include "tc.h"
#include <ctype.h>

typedef void (__cdecl *fl_t)(void *);
static fl_t p_fl;

TC_CASE(null_noop)
{
    volatile int c = 'a'; int n = tc_iph_count;
    p_fl(NULL);
    CHECK_EQ_I(tc_iph_count, n);
    CHECK(isalpha(c) != 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_fl = (fl_t)tc_crt_proc("_free_locale");
    if (!p_fl) { tc_write("TC FAIL " TC_TEST_ID "::export missing\n"); return 1; }
    TC_RUN(null_noop);
    return tc_end();
}
