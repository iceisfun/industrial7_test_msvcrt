/* TC
 * id:    exp_neg_create_locale_bad_category_probe
 * area:  exports
 * funcs:
 * procs: _create_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale
 * note:  Probes NULL + errno EINVAL for an out-of-range category (-1, 100) with locale name "C" returns NULL.
 *        (errno is probed separately in exp_neg_create_locale_bad_category_errno_probe.)
 */
#include "tc.h"
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
static create_t p_create;
static volatile int cat;

TC_CASE(category_minus1) { if (!p_create) TC_SKIP("not exported"); tc_iph_install(); cat = -1; CHECK(p_create(cat, "C") == NULL); }
TC_CASE(category_100)    { if (!p_create) TC_SKIP("not exported"); tc_iph_install(); cat = 100; CHECK(p_create(cat, "C") == NULL); }
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale");
    TC_RUN(category_minus1); TC_RUN(category_100);
    return tc_end();
}
