/* TC
 * id:    exp_neg_free_locale_null_probe
 * area:  exports
 * funcs:
 * procs: _free_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _free_locale
 * note:  Probes that _free_locale(NULL) is accepted silently: no crash and no invalid-parameter handler call.
 */
#include "tc.h"

typedef void (__cdecl *free_t)(void *);
static free_t p_free;
static void *volatile np;

TC_CASE(no_crash_no_iph)
{
    if (!p_free) TC_SKIP("_free_locale not exported");
    tc_iph_install(); tc_iph_count = 0;
    p_free(np);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_free = (free_t)tc_crt_proc("_free_locale");
    TC_RUN(no_crash_no_iph);
    return tc_end();
}
